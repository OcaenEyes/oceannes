#if !defined(MAPPERNROM_H)
#define MAPPERNROM_H

#include "Mapper.h"

class MapperNROM : public Mapper
{
private:
    /* data */
    bool m_oneBank;
    bool m_usesCharacterRAM;

    std::vector<std::uint8_t> m_characterRAM;

public:
    MapperNROM(Cartridge &cart);

    void writePRG(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readPRG(std::uint16_t addr);

    void writeCHR(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readCHR(std::uint16_t addr);

    ~MapperNROM();
};

#endif // MAPPERNROM_H
