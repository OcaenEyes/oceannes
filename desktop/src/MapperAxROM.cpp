/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-20 00:22:08
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-20 00:41:42
 * @FilePath: /oceannes/desktop/src/MapperAxROM.cpp
 * @Description: 注释信息
 */
#include "MapperAxROM.h"
#include "LogUtil.h"

MapperAxROM::MapperAxROM(Cartridge &cart, std::function<void(void)> mirroring_cb) : Mapper(cart, Mapper::AxROM),
                                                                                    m_mirroring(OneScreenHighter),
                                                                                    m_mirroringCallback(mirroring_cb),
                                                                                    m_prgBank(0)
{
    if (cart.getROM().size() >= 0x8000)
    {
        /* code */
        LOG(Info) << "using prg rom ok" << std::endl;
    }
    if (cart.getVROM().size() == 0)
    {
        /* code */
        m_characterRAM.resize(0x2000);
        LOG(Info) << "use character ram ok" << std::endl;
    }
}

MapperAxROM::~MapperAxROM() {}

std::uint8_t MapperAxROM::readPRG(std::uint16_t addr)
{
    if (addr >= 0x8000)
    {
        /* code */
        return m_cartridge.getROM()[m_prgBank * 0x8000 + (addr & 0x7FFF)];
    }

    return 0;
}

void MapperAxROM::writePRG(std::uint16_t addr, std::uint8_t value)
{
    if (addr >= 0x8000)
    {
        /* code */
        m_prgBank = value & 0x07;
        m_mirroring = (value & 0x10) ? OneScreenHighter : OneScreenLower;

        m_mirroringCallback();
    }
}

NameTableMirroring MapperAxROM::getNameTableMirroring()
{
    return m_mirroring;
}

std::uint8_t MapperAxROM::readCHR(std::uint16_t addr)
{
    if (addr < 0x2000)
    {
        /* code */
        return m_characterRAM[addr];
    }
    return 0;
}

void MapperAxROM::writeCHR(std::uint16_t addr, std::uint8_t value)
{
    if (addr < 0x2000)
    {
        /* code */
        m_characterRAM[addr] = value;
    }
}