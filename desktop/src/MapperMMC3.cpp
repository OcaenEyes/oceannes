/*
 * @Author: OCEAN.GZY
 * @Date: 2023-03-14 18:24:49
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-03-14 19:01:41
 * @FilePath: \oceannes\desktop\src\MapperMMC3.cpp
 * @Description: 注释信息
 */
#include "MapperMMC3.h"
#include "LogUtil.h"

MapperMMC3::MapperMMC3(Cartridge &cart, std::function<void()> interrupt_cb, std::function<void(void)> mirroring_cb) : Mapper(cart, Mapper::MMC3),
                                                                                                                      m_targetRegister(0),
                                                                                                                      m_prgBankMode(false),
                                                                                                                      m_chrInversion(false),
                                                                                                                      m_bankRegister{},
                                                                                                                      m_irqEnabled(false),
                                                                                                                      m_irqCounter(0),
                                                                                                                      m_irqLatch(0),
                                                                                                                      m_irqReloadPending(false),
                                                                                                                      m_prgRam(32 * 1024),
                                                                                                                      m_mirroringRam(Horizontal),
                                                                                                                      m_mirroringCallback(mirroring_cb),
                                                                                                                      m_interruptCallback(interrupt_cb)
{

    m_prgBank0 = &cart.getROM()[cart.getROM().size() - 0x4000];
    m_prgBank1 = &cart.getROM()[cart.getROM().size() - 0x2000];
    m_prgBank2 = &cart.getROM()[cart.getROM().size() - 0x4000];
    m_prgBank3 = &cart.getROM()[cart.getROM().size() - 0x2000];

    for (auto &bank : m_chrBanks)
    {
        /* code */
        bank = cart.getVROM().size() - 0x400;
    }
    m_chrBanks[0] = cart.getVROM().size() - 0x800;
    m_chrBanks[3] = cart.getVROM().size() - 0x800;
}

std::uint8_t MapperMMC3::readCHR(std::uint16_t addr)
{
    if (addr < 0x1fff)
    {
        /* code */
        const auto bankSelect = addr >> 10;

        const auto baseAddress = m_chrBanks[bankSelect];
        const auto offset = addr & 0x3ff;
        return m_cartridge.getVROM()[baseAddress + offset];
    }
    else if (addr <= 0x2fff)
    {
        /* code */
        return m_mirroringRam[addr - 0x2000];
    }
    return 0;
}

void MapperMMC3::writeCHR(std::uint16_t addr, std::uint8_t value)
{
    if (addr >= 0x2000 && addr <= 0x2fff)
    {
        /* code */
        m_mirroringRam[addr = 0x2000] = value;
    }
}

std::uint8_t MapperMMC3::readPRG(std::uint16_t addr)
{
    if (addr >= 0x6000 && addr <= 0x7fff)
    {
        /* code */
        return m_prgRam[addr & 0x1fff];
    }

    if (addr >= 0x8000 && addr <= 0x9fff)
    {
        /* code */
        return *(m_prgBank0 + (addr & 0x1fff));
    }

    if (addr >= 0xA000 && addr <= 0xBfff)
    {
        /* code */
        return *(m_prgBank1 + (addr & 0x1fff));
    }

    if (addr >= 0xC000 && addr <= 0xDfff)
    {
        /* code */
        return *(m_prgBank2 + (addr & 0x1fff));
    }

    if (addr >= 0xE000)
    {
        /* code */
        return *(m_prgBank3 + (addr & 0x1fff));
    }

    return 0;
}

void MapperMMC3::writePRG(std::uint16_t addr, std::uint8_t value)
{
}

NameTableMirroring MapperMMC3::getNameTableMirroring()
{
    return m_mirroring;
}

void MapperMMC3::scanlineIRQ()
{
    bool zeroTransition = false;
    if (m_irqCounter == 0 || m_irqReloadPending)
    {
        /* code */
        m_irqCounter = m_irqLatch;
        m_irqReloadPending = false;
    }
    else
    {
        m_irqCounter--;
        zeroTransition = m_irqCounter == 0;
    }

    if (zeroTransition && m_irqEnabled)
    {
        /* code */
        m_interruptCallback();
    }
}