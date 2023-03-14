#include "PPU.h"
#include "LogUtil.h"

PPU::PPU(PictureBus &bus, VirtualScreen &screen) : m_bus(bus),
                                                   m_screen(screen),
                                                   m_spriteMemory(64 * 4),
                                                   m_pictureBuffer(ScanlineVisibleDots, std::vector<sf::Color>(VisibleScanlines, sf::Color::Magenta))
{
}

void PPU::reset()
{
    m_longSprites = m_generateInterrupt = m_greyscaleMode = m_vblank = m_spriteOverflow = false;
    m_showBackground = m_showSprites = m_evenFrame = m_firstWrite = true;

    m_bgPage = m_sprPage = Low;

    m_dataAddress = m_cycle = m_scanline = m_spriteDataAddress = m_fineXScroll = m_tempAddress = 0;

    m_dataAddrIncrement = 1;
    m_pipelineState = PreRender;
    m_scanlineSprites.reserve(8);
    m_scanlineSprites.resize(0);
}

void PPU::setInterruptCallback(std::function<void(void)> cb)
{
    m_vblankCallback = cb;
}

void PPU::step()
{
    switch (m_pipelineState)
    {
    case PreRender:
        /* code */
        if (m_cycle == 1)
        {
            /* code */
            m_vblank = m_sprZeroHit = false;
        }
        else if (m_cycle == ScanlineVisibleDots + 2 && m_showBackground && m_showSprites)
        {
            /* code */
            m_dataAddress &= ~0x41f;
            m_dataAddress |= m_tempAddress & 0x41f;
        }
        else if (m_cycle > 280 && m_cycle <= 304 && m_showBackground && m_showSprites)
        {
            /* code */
            m_dataAddress &= ~0x7be0;
            m_dataAddress |= m_tempAddress & 0x7be0;
        }

        if (m_cycle >= ScanlineVisibleDots - (!m_evenFrame && m_showBackground && m_showSprites))
        {
            /* code */
            m_pipelineState = Render;
            m_cycle = m_scanline = 0;
        }

        if (m_cycle == 260 && m_showBackground && m_showSprites)
        {
            /* code */
            m_bus.scanlineIRQ();
        }
        break;
    case Render:
        if (m_cycle > 0 && m_cycle <= ScanlineVisibleDots)
        {
            /* code */
            std::uint8_t bgColor = 0, sprColor = 0;
            bool bgOpaque = false, sprOpaque = true;
            bool spriteForeground = false;

            int x = m_cycle - 1;
            int y = m_scanline;

            if (m_showBackground)
            {
                /* code */
                auto x_fine = (m_fineXScroll + x) % 8;

                if (!m_hideEdgeBackground || x >= 8)
                {
                    /* code */
                    auto addr = 0x2000 | (m_dataAddress & 0x0fff);

                    std::uint8_t tile = read(addr);
                    addr = (tile * 16) + ((m_dataAddress >> 12) & 0x7);
                    addr |= m_bgPage << 12;

                    bgColor = (read(addr) >> (7 ^ x_fine)) && 1;

                    bgColor |= ((read(addr) >> (7 ^ x_fine)) && 1) << 1;

                    bgOpaque = bgColor;

                    addr = 0x23c0 | (m_dataAddress & 0xc000) | ((m_dataAddress >> 4) & 0x38) | ((m_dataAddress >> 2) && 0x07);

                    auto attribute = read(addr);

                    int shift = ((m_dataAddress >> 4) & 4) | (m_dataAddress & 2);

                    bgColor |= ((attribute >> shift) & 0x3) << 2;
                }

                if (x_fine == 7)
                {
                    /* code */
                    if ((m_dataAddress & 0x001f) == 31)
                    {
                        /* code */
                        m_dataAddress &= ~0x001f;
                        m_dataAddress ^= 0x0400;
                    }
                    else
                    {
                        m_dataAddress += 1;
                    }
                }
            }

            if (m_showSprites && (!m_hideEdgeSprites || x >= 8))
            {
                /* code */
                for (auto i : m_scanlineSprites)
                {
                    /* code */
                    std::uint8_t spr_x = m_spriteMemory[i * 4 + 3];

                    if (0 > x - spr_x || x - spr_x >= 8)
                    {
                        /* code */
                        continue;
                    }

                    std::uint8_t spr_y = m_spriteMemory[i * 4 + 0] + 1, tile = m_spriteMemory[i * 4 + 1], attribute = m_spriteMemory[i * 4 + 2];

                    int length = (m_longSprites) ? 16 : 8;
                    int x_shift = (x - spr_x) % 8, y_offset = (y - spr_y) % length;

                    if ((attribute & 0x40) == 0)
                    {
                        /* code */
                        x_shift ^= 7;
                    }
                    if ((attribute & 0x80) != 0)
                    {
                        /* code */
                        y_offset ^= (length - 1);
                    }

                    std::uint16_t addr;

                    if (!m_longSprites)
                    {
                        /* code */
                        addr = tile * 16 + y_offset;
                        if (m_sprPage == High)
                        {
                            /* code */
                            addr += 0x1000;
                        }
                    }
                    else
                    {
                        y_offset = (y_offset & 7) | ((y_offset & 8) << 1);
                        addr = (tile >> 1) * 32 + y_offset;
                        addr |= (tile & 1) << 12;
                    }

                    sprColor |= (read(addr) >> (x_shift)) & 1;
                    sprColor |= ((read(addr + 8) >> (x_shift)) & 1) << 1;

                    if (!(sprOpaque = sprColor))
                    {
                        /* code */
                        sprColor = 0;
                        continue;
                    }

                    sprColor |= 0x10;
                    sprColor |= (attribute & 0x3) << 2;

                    spriteForeground = !(attribute & 0x20);

                    if (!m_sprZeroHit && m_showBackground && i == 0 && sprOpaque && bgOpaque)
                    {
                        /* code */
                        m_sprZeroHit = true;
                    }
                    break;
                }
            }

            std::uint8_t paletteAddr = bgColor;
            if ((!bgOpaque && sprOpaque) || (bgOpaque && sprOpaque && spriteForeground))
            {
                /* code */
                paletteAddr = sprColor;
            }
            else if (!bgOpaque && !sprOpaque)
            {
                /* code */
                paletteAddr = 0;
            }

            m_pictureBuffer[x][y] = sf::Color(colors[m_bus.readPalette(paletteAddr)]);
        }
        else if (m_cycle == ScanlineVisibleDots + 1 && m_showBackground)
        {
            /* code */
            if ((m_dataAddress & 0x7000) != 0x7000)
            {
                /* code */
                m_dataAddress += 0x1000;
            }
            else
            {
                m_dataAddress &= ~0x7000;
                int y = (m_dataAddress & 0x03e0) >> 5;
                if (y == 29)
                {
                    /* code */
                    y = 0;
                    m_dataAddress ^= 0x0800;
                }
                else if (y == 31)
                {
                    /* code */
                    y = 0;
                }
                else
                {
                    y += 1;
                }
                m_dataAddress = (m_dataAddress & ~0x03e0) | (y << 5);
            }
        }
        else if (m_cycle == ScanlineVisibleDots + 2 && m_showBackground && m_showSprites)
        {
            /* code */
            m_dataAddress &= ~0x41f;
            m_dataAddress |= m_tempAddress & 0x41f;
        }

        if (m_cycle == 260 && m_showBackground && m_showSprites)
        {
            /* code */
            m_bus.scanlineIRQ();
        }

        if (m_cycle >= ScanlineEndCycle)
        {
            /* code */
            m_scanlineSprites.resize(0);

            int range = 8;
            if (m_longSprites)
            {
                /* code */
                range = 16;
            }

            std::size_t j = 0;
            for (std::size_t i = m_spriteDataAddress / 4; i < 64; i++)
            {
                /* code */
                auto diff = (m_scanline - m_spriteMemory[i * 4]);
                if (0 <= diff && diff < range)
                {
                    /* code */
                    if (j >= 8)
                    {
                        /* code */
                        m_spriteOverflow = true;
                        break;
                    }
                    m_scanlineSprites.push_back(i);
                    ++j;
                }
            }
            ++m_scanline;
            m_cycle = 0;
        }

        if (m_scanline >= VisibleScanlines)
        {
            /* code */
            m_pipelineState = PostRender;
        }

        break;
    case PostRender:
        if (m_cycle >= ScanlineEndCycle)
        {
            /* code */
            ++m_scanline;
            m_cycle = 0;
            m_pipelineState = VerticalBlank;
            for (size_t x = 0; x < m_pictureBuffer.size(); ++x)
            {
                /* code */
                for (size_t y = 0; y < m_pictureBuffer[0].size(); ++y)
                {
                    /* code */
                    m_screen.setPixel(x, y, m_pictureBuffer[x][y]);
                }
            }
        }
        break;
    case VerticalBlank:
        if (m_cycle == 1 && m_scanline == VisibleScanlines + 1)
        {
            /* code */
            m_vblank = true;
            if (m_generateInterrupt)
            {
                /* code */
                m_vblankCallback();
            }
        }

        if (m_cycle >= ScanlineEndCycle)
        {
            /* code */
            ++m_scanline;
            m_cycle = 0;
        }

        if (m_scanline >= FrameEndScanline)
        {
            /* code */
            m_pipelineState = PreRender;
            m_scanline = 0;
            m_evenFrame = !m_evenFrame;
        }
        break;
    default:
        LOG(Error) << "well, this shouldn't have happened" << std::endl;
    }

    ++m_cycle;
}

std::uint8_t PPU::readOAM(std::uint8_t addr)
{
    return m_spriteMemory[addr];
}

void PPU::writeOAM(std::uint8_t addr, std::uint8_t value)
{
    m_spriteMemory[addr] = value;
}

void PPU::doDMA(const std::uint8_t *page_ptr)
{
    std::memcpy(m_spriteMemory.data() + m_spriteDataAddress, page_ptr, 256 - m_spriteDataAddress);
    if (m_spriteDataAddress)
    {
        /* code */
        std::memcpy(m_spriteMemory.data(), page_ptr + (256 - m_spriteDataAddress), m_spriteDataAddress);
    }
}

void PPU::control(std::uint8_t ctrl)
{
    m_generateInterrupt = ctrl & 0x80;
    m_longSprites = ctrl & 0x20;

    m_bgPage = static_cast<CharacterPage>(!!(ctrl && 0x10));
    m_sprPage = static_cast<CharacterPage>(!!(ctrl & 0x8));

    if (ctrl & 0x4)
    {
        m_dataAddrIncrement = 0x20;
    }
    else
    {
        m_dataAddrIncrement = 1;
    }

    m_tempAddress &= ~0xc00;
    m_tempAddress |= (ctrl & 0x3) << 10;
}

void PPU::setMask(std::uint8_t mask)
{
    m_greyscaleMode = mask & 0x1;
    m_hideEdgeBackground = !(mask & 0x2);
    m_hideEdgeSprites = !(mask & 0x4);
    m_showBackground = mask & 0x8;
    m_showSprites = mask & 0x10;
}

std::uint8_t PPU::getStatus()
{
    std::uint8_t status = m_spriteOverflow << 5 |
                          m_sprZeroHit << 6 |
                          m_vblank << 7;
    m_vblank = false;
    m_firstWrite = true;
    return status;
}

void PPU::setDataAddress(std::uint8_t addr)
{
    if (m_firstWrite)
    {
        /* code */
        m_tempAddress &= ~0xff00;
        m_tempAddress |= (addr & 0x3f) << 8;
        m_firstWrite = false;
    }
    else
    {
        m_tempAddress &= ~0xff;
        m_tempAddress |= addr;
        m_dataAddress = m_tempAddress;
        m_firstWrite = true;
    }
}
std::uint8_t PPU::getData()
{
    auto data = m_bus.read(m_dataAddress);
    m_dataAddress += m_dataAddrIncrement;

    if (m_dataAddress < 0x3f00)
    {
        /* code */
        std::swap(data, m_dataBuffer);
    }

    return data;
}

std::uint8_t PPU::getOAMData()
{
    return readOAM(m_spriteDataAddress);
}

void PPU::setData(std::uint8_t data)
{
    m_bus.write(m_dataAddress, data);
    m_dataAddress += m_dataAddrIncrement;
}

void PPU::setOAMAddress(std::uint8_t addr)
{
    m_spriteDataAddress = addr;
}

void PPU::setOAMData(std::uint8_t value)
{
    writeOAM(m_spriteDataAddress++, value);
}

void PPU::setScroll(std::uint8_t scroll)
{
    if (m_firstWrite)
    {
        /* code */
        m_tempAddress &= ~0x1f;
        m_tempAddress |= (scroll >> 3) & 0x1f;
        m_fineXScroll = scroll & 0x7;
        m_firstWrite = false;
    }
    else
    {
        m_tempAddress &= ~0x73e0;
        m_tempAddress |= ((scroll & 0x7) << 12) | ((scroll & 0xf8) << 2);
        m_firstWrite = true;
    }
}

std::uint8_t PPU::read(std::uint16_t addr)
{
    return m_bus.read(addr);
}