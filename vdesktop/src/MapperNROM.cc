/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:30:09
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-20 05:40:05
 * @FilePath: /vdesktop/src/MapperNROM.cc
 * @Description: 注释信息
 */
#include "MapperNROM.h"
#include "Log.h"

MapperNROM::MapperNROM(Cartridge &cart) : Mapper(cart, Mapper::NROM)
{
    // 0x4000 =16 KB
    if (cart.GetRom().size() == 0x4000)
    {
        m_one_bank = true;
    }
    // 如果 CHR 为 0 的话，说明卡带上的 CHR 为 RAM 而非 ROM
    if (cart.GetVRom().size() == 0)
    {
        m_use_character_RAM = true;
        m_character_RAM.resize(0x2000);
        LOG(INFO) << "Use character RAM\n";
    }
    else
    {
        LOG(INFO) << "Use CHR-ROM\n";
        m_use_character_RAM = false;
    }
}

MapperNROM::~MapperNROM()
{
}

void MapperNROM::WritePRG(Address addr, Byte value)
{
    LOG(INFO) << "ROM memory write attempt at " << +addr << " to set " << +value << "\n";
}

Byte MapperNROM::ReadPRG(Address addr)
{
    if (!m_one_bank)
    {
        return m_cartridge.GetRom()[addr - 0x8000];
    }
    else
    { // 镜像读取
        return m_cartridge.GetRom()[(addr - 0x8000) & 0x3fff];
    }
}

void MapperNROM::WriteCHR(Address addr, Byte value)
{
    if (m_use_character_RAM)
    {
        m_character_RAM[addr] = value;
    }
    else
    {
        LOG(INFO) << "Read-only CHR memory write attempt at " << std::hex << addr << "\n";
    }
}

Byte MapperNROM::ReadCHR(Address addr)
{
    if (m_use_character_RAM)
    {
        return m_character_RAM[addr];
    }
    else
    {
        return m_cartridge.GetVRom()[addr];
    }
}
