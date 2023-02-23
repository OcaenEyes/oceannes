/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-21 16:01:33
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-21 16:07:37
 * @FilePath: /oceannes/desktop/src/MapperUxROM.cpp
 * @Description: 注释信息
 */
#include "MapperUxROM.h"
#include "LogUtil.h"

MapperUxROM::MapperUxROM(Cartridge &cart) : Mapper(cart, Mapper::UxROM),
                                            m_selectPRG(0)
{
    if (cart.getVROM().size() == 0)
    {
        /* code */
        m_usesCharacterRAM = true;
        m_characterRAM.resize(0x2000);

        LOG(Info) << "uses character ram" << std::endl;
    }
    else
    {
        m_usesCharacterRAM = false;
    }

    m_lastBankPtr = &cart.getROM()[cart.getROM().size() - 0x4000];
}

std::uint8_t MapperUxROM ::readPRG(std::uint16_t addr)
{
    if (addr < 0xc000)
    {
        /* code */
        return m_cartridge.getROM()[(addr - 0x8000) & 0x3fff];
    }
    else
    {
        return *(m_lastBankPtr + (addr & 0x3fff));
    }
}

void MapperUxROM::writePRG(std::uint16_t addr, std::uint8_t value)
{
 m_selectPRG = value;
}

std::uint8_t MapperUxROM ::readCHR(std::uint16_t addr)
{
    if (m_usesCharacterRAM)
    {
        /* code */
        return m_characterRAM[addr];
    }
    else
    {
        return m_cartridge.getVROM()[addr];
    }
}

void MapperUxROM::writeCHR(std::uint16_t addr, std::uint8_t value)
{
    if (m_usesCharacterRAM)
    {
        /* code */
        m_characterRAM[addr] = value;
    }
    else
    {
        LOG(Info) << "read-only chr memory write attempt at" << std::hex << addr << std::endl;
    }
}