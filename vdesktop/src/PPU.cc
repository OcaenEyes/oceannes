/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:30:23
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-25 21:10:54
 * @FilePath: \vdesktop\src\PPU.cc
 * @Description: 注释信息
 */
#include "PPU.h"
#include "Log.h"
#include "PaletteColors.h"

#include <cstring>

/*
 * PPU的实现是NES模拟器最复杂的一部分
 */

PPU::PPU(PictureBus &bus, VirtualScreen &screen) : m_bus(bus),
                                                   m_screen(screen),
                                                   m_sprite_memory(64 * 4),
                                                   m_picture_buffer(ScanlineVisibleDots, std::vector<sf::Color>(VisibleScanlines, sf::Color::Magenta))
{
    // VisibleScanlines 红白机 240
    // Magenta 品红
}

PPU::~PPU()
{
}

void PPU::Reset()
{
    m_long_sprites = m_generate_interrupt = m_greyscale_mode = m_vblank = false;
    m_show_background = m_show_sprites = m_even_frame = m_first_write = true;
    m_bg_page = m_spr_page = Low;
    m_data_address = m_cycle = m_scanline = m_sprite_data_address = m_fine_x_sroll = m_temp_address = 0;

    m_data_addr_increment = 1;
    m_pipeline_state = PreRender;
    m_scanline_sprites.reserve(8);
    m_scanline_sprites.resize(0);

}

void PPU::Step()
{
    switch (m_pipeline_state)
    {
    case PreRender:
        if (m_cycle == 1)
        {
            m_vblank = m_spr_zreo_hit = false;
        }
        else if (m_cycle == ScanlineVisibleDots + 2 && m_show_background && m_show_sprites)
        {
            // Set bits related to horizontal position
            m_data_address &= ~0x41f;                 // Unset horizontal bits
            m_data_address |= m_temp_address & 0x41f; // copy
        }
        else if (m_cycle > 280 && m_cycle <= 304 && m_show_background && m_show_sprites)
        {
            // set Vertical bits
            m_data_address &= 0x7be0; // unset bits related to horizontal
            m_data_address |= m_temp_address & 0x7be0;
        }

        if (m_cycle >= ScanlineEndCycle - (!m_even_frame && m_show_background && m_show_sprites))
        {
            m_pipeline_state = Render;
            m_cycle = m_scanline = 0;
        }

        break;
    case Render:
        if (m_cycle > 0 && m_cycle <= ScanlineVisibleDots)
        {
            Byte bg_color = 0, spr_color = 0;
            bool bg_opaque = false, spr_opaque = true;
            bool sprite_foreground = false;

            int x = m_cycle - 1;
            int y = m_scanline;

            if (m_show_background)
            {
                auto x_fine = (m_fine_x_sroll + x) % 8;
                if (!m_hide_edge_background || x >= 8)
                {
                    // fetch tile
                    auto addr = 0x2000 | (m_data_address & 0xFFF); // mask off fine y
                    Byte tile = Read(addr);

                    // fetch pattern
                    // each pattern occupies 16 bytes, so multiply by 16
                    addr = (tile * 16) + ((m_data_address >> 12) & 0x7);
                    addr |= m_bg_page << 12;
                    // Get the corresponding bit determined by (8 - x_fine) from the right
                    bg_color = (Read(addr) >> (7 ^ x_fine)) & 1;             // bit 0 of palette entry
                    bg_color |= ((Read(addr + 8) >> (7 ^ x_fine)) & 1) << 1; // bit 1

                    bg_opaque = bg_color; // flag used to caculate final pixel with the sprite pixel

                    // fetch attribute and caculate the higher two bits of palette
                    addr = 0x23C0 | (m_data_address & 0xC00) | ((m_data_address >> 4) & 0x38) | ((m_data_address >> 2) & 0x07);

                    auto attribute = Read(addr);
                    int shift = ((m_data_address >> 4) & 4) | (m_data_address & 2);

                    // 提取并设置颜色的 upper two bits
                    bg_color |= ((attribute >> shift) & 0x3) << 2;
                }

                // Increment/wrap coarse X
                if (x_fine == 7)
                {
                    if ((m_data_address & 0x001F) == 31) // if coarse X ==31
                    {
                        m_data_address &= ~0x001F; // coarse X =0
                        m_data_address ^= 0x0400;  // switch horizontal nametable
                    }
                    else
                    {
                        m_data_address += 1; // increment coarse X
                    }
                }

            }

            if (m_show_sprites && (!m_hide_edge_sprites || x >= 8))
            {
                for (auto i : m_scanline_sprites)
                {
                    Byte spr_x = m_sprite_memory[i * 4 + 3];

                    if (x - spr_x < 0 || x - spr_x >= 8)
                    {
                        continue;
                    }

                    Byte spr_y = m_sprite_memory[i * 4 + 0] + 1;
                    Byte tile = m_sprite_memory[i * 4 + 1];
                    Byte attribute = m_sprite_memory[i * 4 + 2];

                    int length = (m_long_sprites) ? 16 : 8;
                    int x_shift = (x - spr_x) % 8, y_offset = (y - spr_y) % length;

                    if ((attribute & 0x40) == 0) // if not flipping horzontally
                    {
                        x_shift ^= 7;
                    }

                    if ((attribute & 0x80) != 0) // if  flipping vertically
                    {
                        y_offset ^= (length - 1);
                    }

                    Address addr = 0;
                    if (!m_long_sprites)
                    {
                        addr = tile * 16 + y_offset;
                        if (m_spr_page == High)
                        {
                            addr += 0x1000;
                        }
                    }
                    else // 8 * 16 sprites
                    {
                        // bit-3 is one if it is the bottom tile of sprite , multiply by two to get the next pattern
                        y_offset = (y_offset & 7) | ((y_offset & 8) << 1);
                        addr = (tile >> 1) * 32 + y_offset;
                        addr |= (tile & 1) << 12; // Bank 0x1000 if bit-0 is high
                    }

                    spr_color |= (Read(addr) >> (x_shift)) & 1;            // bit 0 of palette entry
                    spr_color |= ((Read(addr + 8) >> (x_shift)) & 1) << 1; // bit 1

                    if (!(spr_opaque = spr_color))
                    {
                        spr_color = 0;
                        continue;
                    }

                    spr_color |= 0x10;                   // select sprite palette
                    spr_color |= (attribute & 0x3) << 2; // bits 2-3

                    // Sprite-0 hit detection
                    if (!m_spr_zreo_hit && m_show_background && i == 0 && spr_opaque && bg_opaque)
                    {
                        m_spr_zreo_hit = true;
                    }

                    break; // Exit the loop now since we've found the highest priority sprite
                }

            }

            Byte palette_addr = bg_color;

            if ((!bg_opaque && spr_opaque) || (bg_opaque && spr_opaque && sprite_foreground))
            {
                palette_addr = spr_color;
            }
            else if (!bg_opaque && !spr_opaque)
            {
                palette_addr = 0;
            }

            m_picture_buffer[x][y] = sf::Color(colors[m_bus.ReadPalette(palette_addr)]);

        }
        else if (m_cycle == ScanlineVisibleDots + 1 && m_show_background)
        {
            // Shamelessly copied from nesdev wiki
            if ((m_data_address & 0x7000) != 0x7000) // if fine Y < 7
                m_data_address += 0x1000;            // increment fine Y
            else
            {
                m_data_address &= ~0x7000;              // fine Y = 0
                int y = (m_data_address & 0x03E0) >> 5; // let y = coarse Y
                if (y == 29)
                {
                    y = 0;                    // coarse Y = 0
                    m_data_address ^= 0x0800; // switch vertical nametable
                }
                else if (y == 31)
                    y = 0; // coarse Y = 0, nametable not switched
                else
                    y += 1; // increment coarse Y
                m_data_address = (m_data_address & ~0x03E0) | (y << 5);
                // put coarse Y back into m_dataAddress
            }
        }
        else if (m_cycle == ScanlineVisibleDots + 2 && m_show_background && m_show_sprites)
        {
            // Copy bits related to horizontal position
            m_data_address &= ~0x41f;
            m_data_address |= m_temp_address & 0x41f;
        }

        //                 if (m_cycle > 257 && m_cycle < 320)
        //                     m_spriteDataAddress = 0;

        if (m_cycle >= ScanlineEndCycle)
        {
            // Find and index sprites that are on the next Scanline
            // This isn't where/when this indexing, actually copying in 2C02 is done
            // but (I think) it shouldn't hurt any games if this is done here

            m_scanline_sprites.resize(0);

            int range = 8;
            if (m_long_sprites)
                range = 16;

            std::size_t j = 0;
            for (std::size_t i = m_sprite_data_address / 4; i < 64; ++i)
            {
                auto diff = (m_scanline - m_sprite_memory[i * 4]);
                if (0 <= diff && diff < range)
                {
                    m_scanline_sprites.push_back(i);
                    ++j;
                    if (j >= 8)
                    {
                        break;
                    }
                }
            }

            ++m_scanline;
            m_cycle = 0;
        }

        if (m_scanline >= VisibleScanlines)
            m_pipeline_state = PostRender;

        break;
    case PostRender:
        if (m_cycle >= ScanlineEndCycle)
        {
            ++m_scanline;
            m_cycle = 0;
            m_pipeline_state = VerticalBlank;

            for (int x = 0; x < m_picture_buffer.size(); ++x)
            {
                // 直接用[0]的size来索引了
                for (int y = 0; y < m_picture_buffer[0].size(); ++y)
                {
                    m_screen.SetPixel(x, y, m_picture_buffer[x][y]);
                }
            }

            // Should technically be done at first dot of VBlank, but this is close enough
            //                     m_vblank = true;
            //                     if (m_generateInterrupt) m_vblankCallback();
        }

        break;
    case VerticalBlank:
        if (m_cycle == 1 && m_scanline == VisibleScanlines + 1)
        {
            m_vblank = true;
            if (m_generate_interrupt)
                m_vblank_callback();
        }

        if (m_cycle >= ScanlineEndCycle)
        {
            ++m_scanline;
            m_cycle = 0;
        }

        if (m_scanline >= FrameEndScanline)
        {
            m_pipeline_state = PreRender;
            m_scanline = 0;
            m_even_frame = !m_even_frame;
            //                     m_vblank = false;
        }

        break;
    default:
        LOG_ERROR("Well, this shouldn't have happened.");
    }
    ++m_cycle;
}

void PPU::SetInterruptCallback(std::function<void(void)> cb)
{
    m_vblank_callback = cb;
}

void PPU::SetMask(Byte mask)
{
    m_greyscale_mode = mask & 0x1;
    m_hide_edge_background = !(mask & 0x2);
    m_hide_edge_sprites = !(mask & 0x4);
    m_show_background = mask & 0x8;
    m_show_sprites = mask & 0x10;

}

Byte PPU::GetStatus()
{
    Byte status = m_spr_zreo_hit << 6 | m_vblank << 7;
    m_vblank = false;
    m_first_write = true;

    return status;
}

void PPU::SetDataAddress(Byte addr)
{
    if (m_first_write)
    {
        m_temp_address &= ~0xff00; // unset the upper byte
        m_temp_address |= (addr & 0x3f) << 8;
        m_first_write = false;
    }
    else
    {
        m_temp_address &= ~0xff;
        m_temp_address |= addr;
        m_data_address = m_temp_address;
        m_first_write = true;
    }
}

void PPU::SetData(Byte data)
{
    m_bus.Write(m_data_address, data);
    m_data_address += m_data_addr_increment;
}

Byte PPU::GetData()
{
    auto data = m_bus.Read(m_data_address);
    m_data_address += m_data_addr_increment;

    // Reads are delayed by one byte/read when address is in this range
    if (m_data_address < 0x3f00)
    {
        // Return from the data buffer and store the current value in the buffer
        std::swap(data, m_data_buffer);
    }

    return data;
}

void PPU::SetOAMAddress(Byte addr)
{
    m_sprite_data_address = addr;
}

void PPU::SetOAMData(Byte value)
{
    WriteOAM(m_sprite_data_address++, value);
}

Byte PPU::GetOAMData()
{
    return ReadOAM(m_sprite_data_address);
}

void PPU::SetScroll(Byte scroll)
{
    if (m_first_write)
    {
        m_temp_address &= ~0x1f;
        m_temp_address |= (scroll >> 3) & 0x1f;
        m_fine_x_sroll = scroll & 0x7;
        m_first_write = false;
    }
    else
    {
        m_temp_address &= ~0x73e0;
        m_temp_address |= ((scroll & 0x7) << 12) | ((scroll & 0xf8) << 2);
        m_first_write = true;
    }
}

void PPU::Control(Byte ctl)
{
    m_generate_interrupt = ctl & 0x80;
    m_long_sprites = ctl & 0x20;

    m_bg_page = static_cast<CharacterPage>(!!(ctl & 0x10));
    m_spr_page = static_cast<CharacterPage>(!!(ctl & 0x8));
    if (ctl & 0x4)
    {
        m_data_addr_increment = 0x20;
    }
    else
    {
        m_data_addr_increment = 1;
    }

    // Set the nametable in the temp address, this will be reflected in the data address during rendering
    m_temp_address &= ~0xc00;            // Unset
    m_temp_address |= (ctl & 0x3) << 10; // Set according to ctrl bits
}

void PPU::DoDMA(const Byte *page_ptr)
{
    std::memcpy(m_sprite_memory.data() + m_sprite_data_address, page_ptr, 256 - m_sprite_data_address);
    if (m_sprite_data_address)
    {
        std::memcpy(m_sprite_memory.data(), page_ptr + (256 - m_sprite_data_address), m_sprite_data_address);
    }
}

Byte PPU::Read(Address addr)
{
    return m_bus.Read(addr);
}

Byte PPU::ReadOAM(Byte addr)
{
    return m_sprite_memory[addr];
}

void PPU::WriteOAM(Byte addr, Byte value)
{
    m_sprite_memory[addr] = value;
}
