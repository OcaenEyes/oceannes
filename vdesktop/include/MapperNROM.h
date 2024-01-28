/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:25:48
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-28 12:01:28
 * @FilePath: /vdesktop/include/MapperNROM.h
 * @Description: 注释信息
 */
#pragma once

#include "Mapper.h"

class MapperNROM : public Mapper
{
public:
    MapperNROM(Cartridge &cart);
    void WritePRG(Address addr, Byte value);
    Byte ReadPRG(Address addr);

    Byte ReadCHR(Address addr);
    void WriteCHR(Address addr, Byte value);

private:
    bool m_oneBank;
    bool m_usesCharacterRAM;

    std::vector<Byte> m_characterRAM;
};