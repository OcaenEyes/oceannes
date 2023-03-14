/*
 * @Author: OCEAN.GZY
 * @Date: 2023-03-14 16:14:01
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-03-14 16:33:12
 * @FilePath: \oceannes\desktop\src\MapperColorDreams.cpp
 * @Description: 注释信息
 */
#include "MapperColorDreams.h"
#include "LogUtil.h"

MapperColorDreams::MapperColorDreams(Cartridge &cart, std::function<void(void)> mirroring_cb) : Mapper(cart, Mapper::ColorDreams),
                                                                                                m_mirroring(Vertical),
                                                                                                m_mirroringCallback(mirroring_cb) {}

std::uint8_t MapperColorDreams::readPRG(std::uint16_t addr)
{
    if (addr >= 0x8000)
    {
        return m_cartridge.getROM()[(prgBank * 0x8000) + (addr & 0x7fff)];
    }
    return 0;
}

void MapperColorDreams::writePRG(std::uint16_t addr, std::uint8_t value)
{
    if (addr >= 0x8000)
    {
        /* code */
        prgBank = ((value >> 0) & 0x3);
        chrBank = ((value >> 4) & 0xf);
    }
}

std::uint8_t MapperColorDreams::readCHR(std::uint16_t addr)
{
    if (addr <= 0x1fff)
    {
        return m_cartridge.getVROM()[(chrBank * 0x2000) + addr];
    }
    return 0;
}

void MapperColorDreams::writeCHR(std::uint16_t addr, std::uint8_t value)
{
}

NameTableMirroring MapperColorDreams::getNameTableMirroring()
{
    return m_mirroring;
}