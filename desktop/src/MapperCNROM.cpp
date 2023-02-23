/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-20 23:42:00
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-21 00:01:05
 * @FilePath: /oceannes/desktop/src/MapperCNROM.cpp
 * @Description: 注释信息
 */
#include "MapperCNROM.h"
#include "LogUtil.h"

MapperCNROM::MapperCNROM(Cartridge &cart) : Mapper(cart, Mapper::CNROM), m_selectCHR(0)
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
}

std::uint8_t MapperCNROM::readPRG(std::uint16_t addr)
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

void MapperCNROM::writePRG(std::uint16_t addr, std::uint8_t value)
{
    m_selectCHR = value & 0x3;
}

std::uint8_t MapperCNROM::readCHR(std::uint16_t addr)
{
    return m_cartridge.getVROM()[addr | (m_selectCHR << 13)];
}

void MapperCNROM::writePRG(std::uint16_t addr, std::uint8_t value)
{
    LOG(Info) << "read-only chr memory  write attempt at" << std::hex << addr << std::endl;
}