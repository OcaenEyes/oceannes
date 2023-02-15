/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 19:28:20
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-15 20:05:14
 * @FilePath: /oceannes/desktop/include/MapperAxROM.h
 * @Description: 注释信息
 */

#if !defined(MAPPERAXROM_H)
#define MAPPERAXROM_H

#include "Mapper.h"
#include "PictureBus.h"

class MapperAxROM : public Mapper
{
private:
    /* data */
    NameTableMirroring m_mirroring;
    std::function<void(void)> m_mirroringCallback;
    std::uint32_t m_prgBank;
    std::vector<std::uint8_t> m_characterRAM;

public:
    MapperAxROM(Cartridge &cart, std::function<void(void)> mirroring_cb);

    void writePRG(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readPRG(std::uint16_t addr);

    void writeCHR(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readCHR(std::uint16_t addr);

    NameTableMirroring getNameTableMirroring();

    ~MapperAxROM();
};

#endif // MAPPERAXROM_H
