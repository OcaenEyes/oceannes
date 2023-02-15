/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 23:17:56
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-15 23:36:09
 * @FilePath: /oceannes/desktop/include/MapperGxROM.h
 * @Description: 注释信息
 */
#if !defined(MAPPERGXROM_H)
#define MAPPERGXROM_H

#include "Mapper.h"
class MapperGxROM : public Mapper
{
private:
    /* data */
    NameTableMirroring m_mirroring;
    std::vector<std::uint8_t> m_characterRAM;
    std::function<void(void)> m_mirroringCallback;

public:
    MapperGxROM(Cartridge &cart, std::function<void(void)> mirroring_cb); //语法：std::function<return_type(args_type)>

    ~MapperGxROM();

    void writePRG(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readPRG(std::uint16_t addr);

    void writeCHR(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readCHR(std::uint16_t addr);

    NameTableMirroring getNameTableMirroring();

    std::uint32_t prgBank;
    std::uint32_t chrBank;
};

#endif // MAPPERGXROM_H
