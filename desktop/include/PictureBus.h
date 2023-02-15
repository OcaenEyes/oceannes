/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 10:12:40
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-15 11:59:08
 * @FilePath: /oceannes/desktop/include/PictureBus.h
 * @Description: 注释信息
 */
#if !defined(PICTUREBUS_H)
#define PICTUREBUS_H
#include <vector>
#include "Mapper.h"
#include "Cartridge.h"

class PictureBus
{
private:
    /* data */
    std::size_t NameTable0, NameTable1, NameTable2, NameTable3;

    std::vector<std::uint8_t> m_palette;
    std::vector<std::uint8_t> m_RAM;
    Mapper *m_mapper;

public:
    PictureBus(/* args */);
    ~PictureBus();

    std::uint8_t read(std::uint16_t addr);
    void write(std::uint16_t addr, std::uint8_t value);

    bool setMapper(Mapper *mapper);
    std::uint8_t readPalette(std::uint8_t paletteAddr);
    void updateMirroring();
    void scanlineIRQ();
};

PictureBus::PictureBus(/* args */)
{
}

PictureBus::~PictureBus()
{
}

#endif // PICTUREBUS_H
