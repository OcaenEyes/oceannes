/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-21 11:18:37
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-21 16:00:45
 * @FilePath: /oceannes/desktop/src/MapperNROM.cpp
 * @Description: 注释信息
 */
#include "MapperNROM.h"
#include "LogUtil.h"

MapperNROM ::MapperNROM(Cartridge &cart) : Mapper(cart, Mapper::NORM) // 将派生类的构造参数，传递给基类的构造函数
{
    if (cart.getROM().size() == 0x4000)
    {
        /* code */
        m_oneBank = true;
    }
    else
    {
        m_oneBank = false;
    }

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
}

MapperNROM::~MapperNROM() {}

std::uint8_t MapperNROM ::readPRG(std::uint16_t addr)
{
    if (!m_oneBank)
    {
        /* code */
        return m_cartridge.getROM()[addr - 0x8000];
    }
    else
    {
        return m_cartridge.getROM()[(addr - 0x8000) & 0x3fff];
    }
}

void MapperNROM::writePRG(std::uint16_t addr, std::uint8_t value)
{
    LOG(InfoVerbose) << "ROM memory write attempt at" << +addr << "to set" << +value << std::endl;
}

std::uint8_t MapperNROM ::readCHR(std::uint16_t addr)
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

void MapperNROM::writeCHR(std::uint16_t addr, std::uint8_t value)
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