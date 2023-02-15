/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 00:32:30
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-15 00:51:55
 * @FilePath: /oceannes/desktop/include/Cartridge.h
 * @Description: 注释信息
 */
#if !defined(CARTRIDGE_H)
#define CARTRIDGE_H

#include <vector>
#include <string>
#include <cstdint>

class Cartridge
{
private:
    /* data */
    std::vector<std::uint8_t> m_PRG_ROM;
    std::vector<std::uint8_t> m_CHR_ROM;

    std::uint8_t m_nameTableMirroring;
    std::uint8_t m_mapperNumber;
    bool m_extendedRAM;
    bool m_chrRAM;

public:
    Cartridge(/* args */);
    ~Cartridge();

    bool loadFromFile();

    const std::vector<std::uint8_t> &getROM();
    const std::vector<std::uint8_t> &getVROM();

    std::uint8_t getMapper();

    std::uint8_t getNameTableMirroring();

    bool hasExtendedRAM();
};

#endif // CARTRIDGE_H
