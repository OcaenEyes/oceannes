/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 19:53:13
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-15 19:56:04
 * @FilePath: /oceannes/desktop/include/MapperCNROM.h
 * @Description: 注释信息
 */
#if !defined(MAPPERCNROM_H)
#define MAPPERCNROM_H

#include "Mapper.h"

class MapperCNROM : public Mapper
{
private:
    /* data */
    bool m_oneBank;
    std::uint16_t m_selectCHR;

public:
    MapperCNROM(Cartridge &cart);
    void writePRG(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readPRG(std::uint16_t addr);

    void writeCHR(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readCHR(std::uint16_t addr);

    ~MapperCNROM();
};

#endif // MAPPERCNROM_H
