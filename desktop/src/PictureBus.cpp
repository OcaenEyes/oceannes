#include "PictureBus.h"
#include "LogUtil.h"

PictureBus::PictureBus() : m_palette(0x20),
                           m_RAM(0x800),
                           m_mapper(nullptr) {}

std::uint8_t PictureBus::read(std::uint16_t addr)
{
    if (addr < 0x2000)
    {
        /* code */
        return m_mapper->readCHR(addr);
    }
    else if (addr < 0x3eff)
    {
        /* code */
        const auto index = addr & 0x3ff;
        auto normalizedAddr = addr;

        if (addr >= 0x3000)
        {
            /* code */
            normalizedAddr -= 0x1000;
        }

        if (NameTable0 >= m_RAM.size())
        {
            /* code */
            return m_mapper->readCHR(normalizedAddr);
        }
        else if (normalizedAddr < 0x2400)
        {
            /* code */
            return m_RAM[NameTable0 + index];
        }
        else if (normalizedAddr < 0x2800)
        {
            /* code */
            return m_RAM[NameTable1 + index];
        }
        else if (normalizedAddr < 0x2c00)
        {
            /* code */
            return m_RAM[NameTable2 + index];
        }
        else
        {
            return m_RAM[NameTable3 + index];
        }
    }
    else if (addr < 0x3fff)
    {
        /* code */
        auto paletteAddr = addr & 0x1f;
        return readPalette(paletteAddr);
    }

    return 0;
}

std::uint8_t PictureBus::readPalette(std::uint8_t paletteAddr)
{
    if (paletteAddr >= 0x10 && paletteAddr % 4 == 0)
    {
        /* code */
        paletteAddr = paletteAddr & 0xf;
    }

    return m_palette[paletteAddr];
}

void PictureBus::write(std::uint16_t addr, std::uint8_t value)
{
    if (addr < 0x2000)
    {
        /* code */
        m_mapper->writeCHR(addr, value);
    }
    else if (addr < 0x3eff)
    {
        /* code */
        const auto index = addr & 0x3ff;
        auto normalizedAddr = addr;
        if (addr >= 0x3000)
        {
            /* code */
            normalizedAddr -= 0x1000;
        }

        if (NameTable0 >= m_RAM.size())
        {
            /* code */
            m_mapper->writeCHR(normalizedAddr, value);
        }
        else if (normalizedAddr < 0x2400)
        {
            /* code */
            m_RAM[NameTable0 + index] = value;
        }
        else if (normalizedAddr < 0x2800)
        {
            /* code */
            m_RAM[NameTable1 + index] = value;
        }
        else if (normalizedAddr < 0x2c00)
        {
            /* code */
            m_RAM[NameTable2 + index] = value;
        }
        else
        {
            m_RAM[NameTable3 + index] = value;
        }
    }
    else if (addr < 0x3fff)
    {
        /* code */
        auto palette = addr & 0x1f;
        if (palette >= 0x10 && addr % 4 == 0)
        {
            /* code */
            palette = palette & 0xf;
        }

        m_palette[palette] = value;
    }
}

void PictureBus::scanlineIRQ()
{
    m_mapper->scanlineIRQ();
}

bool PictureBus::setMapper(Mapper *mapper)
{
    if (!mapper)
    {
        /* code */
        LOG(Error) << "Mapper argumer is nullptr" << std::endl;
        return false;
    }

    m_mapper = mapper;
    updateMirroring();
    return true;
}

void PictureBus::updateMirroring()
{
    switch (m_mapper->getNameTableMirroring())
    {
    case Horizontal:
        /* code */
        NameTable0 = NameTable1 = 0;
        NameTable2 = NameTable3 = 0x400;
        LOG(InfoVerbose) << "Horizontal name table mirroring set" << std::endl;
        break;
    case Vertical:
        /* code */
        NameTable0 = NameTable2 = 0;
        NameTable1 = NameTable3 = 0x400;
        LOG(InfoVerbose) << "Vertical name table mirroring set" << std::endl;
        break;
    case OneScreenLower:
        /* code */
        NameTable0 = NameTable1 = NameTable2 = NameTable3 = 0;
        LOG(InfoVerbose) << "Single Screen Lower  mirroring " << std::endl;
        break;
    case OneScreenHighter:
        /* code */
        NameTable0 = NameTable1 = NameTable2 = NameTable3 = 0x400;
        LOG(InfoVerbose) << "Single Screen Higher mirroring " << std::endl;
        break;
    case FourScreen:
        /* code */
        NameTable0 = m_RAM.size();
        LOG(InfoVerbose) << "Four Screen mirroring" << std::endl;
        break;
    default:
        NameTable0 = NameTable1 = NameTable2 = NameTable3 = 0;
        LOG(Error) << "Unsupported Name Table mirroring : " << m_mapper->getNameTableMirroring() << std::endl;
    }
}