/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 19:57:50
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-20 00:18:33
 * @FilePath: /oceannes/desktop/include/MapperColorDreams.h
 * @Description: 注释信息
 */
#if !defined(MAPPERCOLORDREAMS_H)
#define MAPPERCOLORDREAMS_H

#include "Mapper.h"
class MapperColorDreams : public Mapper
{
private:
    /* data */
    NameTableMirroring m_mirroring;
    std::function<void(void)> m_mirroringCallback;
    std::uint32_t prgBank;
    std::uint32_t chrBank;

public:
    MapperColorDreams(Cartridge &cart, std::function<void(void)> mirroring_cb);

    void writePRG(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readPRG(std::uint16_t addr);

    void writeCHR(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readCHR(std::uint16_t addr);

    NameTableMirroring getNameTableMirroring();
    
    ~MapperColorDreams();
};

#endif // MAPPERCOLORDREAMS_H