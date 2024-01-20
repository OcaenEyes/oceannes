#pragma once

#include "Mapper.h"

class MapperNROM : public Mapper
{
private:
    bool m_one_bank;
    bool m_use_character_RAM;

    std::vector<Byte> m_character_RAM;

public:
    MapperNROM(Cartridge &cart);
    ~MapperNROM();

    void WritePRG(Address addr, Byte value);
    Byte ReadPRG(Address addr);

    void WriteCHR(Address addr, Byte value);
    Byte ReadCHR(Address addr);
};
