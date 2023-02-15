/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 23:47:37
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-15 23:50:58
 * @FilePath: /oceannes/desktop/include/MapperUxROM.h
 * @Description: 注释信息
 */
#if !defined(MAPPERUXROM_H)
#define MAPPERUXROM_H

#include "Mapper.h"

class MapperUxROM : public Mapper
{
private:
    /* data */
    bool m_usesCharacterRAM;

    const std::uint8_t *m_lastBankPtr;
    std::uint16_t m_selectPRG;
    std::vector<std::uint8_t> m_characterRAM;

public:
    MapperUxROM(Cartridge &cart);

    void writePRG(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readPRG(std::uint16_t addr);

    void writeCHR(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readCHR(std::uint16_t addr);

    ~MapperUxROM();
};

#endif // MAPPERUXROM_H
