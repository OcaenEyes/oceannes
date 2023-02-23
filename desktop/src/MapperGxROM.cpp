/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-21 00:06:14
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-21 00:21:27
 * @FilePath: /oceannes/desktop/src/MapperGxROM.cpp
 * @Description: 注释信息
 */
#include "MapperGxROM.h"
#include "LogUtil.h"

MapperGxROM::MapperGxROM(Cartridge &cart, std::function<void(void)> mirroring_cb) : Mapper(cart, Mapper::GxROM),
                                                                                    m_mirroring(Vertical),
                                                                                    m_mirroringCallback(mirroring_cb)
{
}

std::uint8_t MapperGxROM::readPRG(std::uint16_t addr)
{
    if (addr >= 0x8000)
    {
        /* code */
        return m_cartridge.getROM()[(prgBank * 0x8000) + (addr & 0x7fff)];
    }

    return 0;
}

void MapperGxROM::writePRG(std::uint16_t addr, std::uint8_t value)
{
    if (addr >= 0x8000)
    {
        /* code */
        prgBank = ((value & 0x30) >> 4);
        chrBank = (value & 0x3);
        m_mirroring = Vertical;
    }
    m_mirroringCallback();
}

std::uint8_t MapperGxROM::readCHR(std::uint16_t addr)
{
    if (addr <= 0x1fff)
    {
        /* code */
        return m_cartridge.getVROM()[chrBank * 0x2000 + addr];
    }
    return 0;
}

NameTableMirroring MapperGxROM::getNameTableMirroring()
{
    return m_mirroring;
}

void MapperGxROM::writeCHR(std::uint16_t addr, std::uint8_t value)
{
    LOG(Info) << "not excepting write here";
}