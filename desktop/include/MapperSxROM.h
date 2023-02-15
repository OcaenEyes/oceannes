/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 23:40:44
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-15 23:46:56
 * @FilePath: /oceannes/desktop/include/MapperSxROM.h
 * @Description: 注释信息
 */
#if !defined(MAPPERSXROM_H)
#define MAPPERSXROM_H

#include "Mapper.h"

class MapperSxROM : public Mapper
{
private:
    /* data */
    void calculatePRGPointers();

    std::function<void(void)> m_mirroringCallback;
    NameTableMirroring m_mirroing;

    bool m_usesCharacterRAM;
    int m_modeCHR;
    int m_modePRG;

    std::uint8_t m_tempRegister;
    int m_writeCounter;

    std::uint8_t m_regPRG;
    std::uint8_t m_regCHR0;
    std::uint8_t m_regCHR1;

    const std::uint8_t *m_firstBankPRG;
    const std::uint8_t *m_secondBankPRG;

    const std::uint8_t *m_firstBankCHR;
    const std::uint8_t *m_secondBankCHR;

    std::vector<std::uint8_t> m_characterRAM;

public:
    MapperSxROM(Cartridge &cart, std::function<void(void)> mirroring_cb);

    void writePRG(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readPRG(std::uint16_t addr);

    void writeCHR(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readCHR(std::uint16_t addr);

    NameTableMirroring getNameTableMirroring();

    ~MapperSxROM();
};

#endif // MAPPERSXROM_H
