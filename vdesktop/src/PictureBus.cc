/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:30:17
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-25 10:55:47
 * @FilePath: /vdesktop/src/PictureBus.cc
 * @Description: 注释信息
 */
#include "PictureBus.h"
#include "Log.h"
#include "PaletteColors.h"

/*
 * http://fms.komkon.org/EMUL8/NES.html
 * PPU Memory Map
--------------------------------------- $4000
 Empty
--------------------------------------- $3F20
 Sprite Palette
--------------------------------------- $3F10
 Image Palette
--------------------------------------- $3F00
 Empty
--------------------------------------- $3000
 Attribute Table 3
--------------------------------------- $2FC0
 Name Table 3 (32x30 tiles)
--------------------------------------- $2C00
 Attribute Table 2
--------------------------------------- $2BC0
 Name Table 2 (32x30 tiles)
--------------------------------------- $2800
 Attribute Table 1
--------------------------------------- $27C0
 Name Table 1 (32x30 tiles)
--------------------------------------- $2400
 Attribute Table 0
--------------------------------------- $23C0
 Name Table 0 (32x30 tiles)
--------------------------------------- $2000
 Pattern Table 1 (256x2x8, may be VROM)
--------------------------------------- $1000
 Pattern Table 0 (256x2x8, may be VROM)
--------------------------------------- $0000

*/

PictureBus::PictureBus() : m_RAM(0x800), m_palette(0x20), m_mapper(nullptr)
{
}

PictureBus::~PictureBus()
{
}

Byte PictureBus::Read(Address addr)
{
    if (addr < 0x2000)
    {
        return m_mapper->ReadCHR(addr);
    }
    else if (addr < 0x3eff)
    {
        auto index = addr & 0x3ff;
        if (addr < 0x2400) // NT0
        {
            return m_RAM[m_name_table0 + index];
        }
        else if (addr < 0x2800) // NT1
        {
            return m_RAM[m_name_table1 + index];
        }
        else if (addr < 0x2c00) // NT2
        {
            return m_RAM[m_name_table2 + index];
        }
        else // NT3
        {
            return m_RAM[m_name_table3 + index];
        }
    }
    else if (addr < 0x3fff)
    {
        /* 0x3f00 - 0x3f20 */
        // 分为image 和 sprite，每个 palette 大小为16
        return m_palette[addr & 0x1f];
    }
    return 0;
}

void PictureBus::Write(Address addr, Byte value)
{
    if (addr < 0x2000)
    {
        m_mapper->WriteCHR(addr, value);
    }
    else if (addr < 0x3eff) // Name tables upto 0x3000, then mirrored upto 3eff
    {
        auto index = addr & 0x3ff;
        if (addr < 0x2400)
        {
            m_RAM[m_name_table0 + index] = value;
        }
        else if (addr < 0x2800)
        {
            m_RAM[m_name_table1 + index] = value;
        }
        else if (addr < 0x2c00)
        {
            m_RAM[m_name_table2 + index] = value;
        }
        else
        {
            m_RAM[m_name_table3 + index] = value;
        }
    }
    else if (addr < 0x3fff)
    {
        if (addr == 0x3f10)
        {
            m_palette[0] = value;
        }
        else
        {
            m_palette[addr & 0x1f] = value;
        }
    }
}

void PictureBus::UpdateMirroring()
{
    switch (m_mapper->GetNameTableMirroring())
    {
    case Horizontal: // 横
        m_name_table0 = m_name_table1 = 0;
        m_name_table2 = m_name_table3 = 0x400;
        LOG_INFO("Horizontal Name table mirroing");
        break;
    case Vertical: // 竖
        m_name_table0 = m_name_table2 = 0;
        m_name_table1 = m_name_table3 = 0x400;
        LOG_INFO("Vertical Name table mirroing");
        break;
    case OneScreenLower:
        m_name_table0 = m_name_table2 = m_name_table1 = m_name_table3 = 0;
        LOG_INFO("Single Screen mirroring set with lower bank.");
        break;
    case OneScreenHigher:
        m_name_table0 = m_name_table2 = m_name_table1 = m_name_table3 = 0x400;
        LOG_INFO("Single Screen mirroring set with higher bank.");
        break;
    default:
        m_name_table0 = m_name_table2 = m_name_table1 = m_name_table3 = 0;
        LOG_ERROR("Unsupported name table mirroring.");
        break;
    }
}

bool PictureBus::SetMapper(Mapper *mapper)
{
    if (!mapper)
    {
        LOG_ERROR("Mapper argument is nullptr.");
        return false;
    }
    m_mapper = mapper;
    UpdateMirroring();
    return true;
}

Byte PictureBus::ReadPalette(Byte palette_addr)
{
    return m_palette[palette_addr];
}
