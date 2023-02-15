#if !defined(MAPPERMMC3_H)
#define MAPPERMMC3_H

#include "Mapper.h"
#include <array>

class MapperMMC3 : public Mapper
{
private:
    /* data */
    std::uint32_t m_targetRegister;
    bool m_prgBankMode;
    bool m_chrInversion;

    std::uint32_t m_bankRegister[8];

    bool m_irqEnabled;
    std::uint8_t m_irqCounter;
    std::uint8_t m_irqLatch;
    bool m_irqReloadPending;

    std::vector<std::uint8_t> m_prgRam;
    std::vector<std::uint8_t> m_mirroringRam;

    const std::uint8_t *m_prgBank0;
    const std::uint8_t *m_prgBank1;
    const std::uint8_t *m_prgBank2;
    const std::uint8_t *m_prgBank3;

    std::array<std::uint32_t,8> m_chrBanks;

    NameTableMirroring m_mirroring;

    std::function<void(void)> m_mirroringCallback; //语法：std::function<return_type(args_type)>

    std::function<void()> m_interruptCallback;

public:
    MapperMMC3(Cartridge &cart, std::function<void()> interrupt_cb, std::function<void(void)> mirroring_cb);  //语法：std::function<return_type(args_type)>


    ~MapperMMC3();

    void writePRG(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readPRG(std::uint16_t addr);

    void writeCHR(std::uint16_t addr, std::uint8_t value);
    std::uint8_t readCHR(std::uint16_t addr);

    NameTableMirroring getNameTableMirroring();

    void scanlineIRQ();
};

#endif // MAPPERMMC3_H
