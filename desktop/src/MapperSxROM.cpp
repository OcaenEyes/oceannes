/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-21 16:08:26
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-03-14 16:08:44
 * @FilePath: \oceannes\desktop\src\MapperSxROM.cpp
 * @Description: 注释信息
 */
#include "MapperSxROM.h"
#include "LogUtil.h"

MapperSxROM::MapperSxROM(Cartridge &cart, std::function<void(void)> mirroring_cb) : Mapper(cart, Mapper::SxROM),
                                                                                    m_mirroringCallback(mirroring_cb),
                                                                                    m_mirroing(Horizontal),
                                                                                    m_modeCHR(0),
                                                                                    m_modePRG(3),
                                                                                    m_tempRegister(0),
                                                                                    m_writeCounter(0),
                                                                                    m_regPRG(0),
                                                                                    m_regCHR0(0),
                                                                                    m_regCHR1(1),
                                                                                    m_firstBankCHR(nullptr),
                                                                                    m_firstBankPRG(nullptr),
                                                                                    m_secondBankCHR(nullptr),
                                                                                    m_secondBankPRG(nullptr)
{
    if (cart.getVROM().size() == 0)
    {
        /* code */
        m_usesCharacterRAM = true;
        m_characterRAM.resize(0x2000);
        LOG(Info) << "uses character ram" << std::endl;
    }
    else
    {
        LOG(Info) << "using CHR-ROM" << std::endl;
        m_usesCharacterRAM = false;

        m_firstBankCHR = &cart.getVROM()[0];
        m_secondBankCHR = &cart.getVROM()[0x1000 * m_regCHR1];
    }

    m_firstBankPRG = &cart.getROM()[0];
    m_secondBankPRG = &cart.getROM()[cart.getROM().size() - 0x4000];
}

std::uint8_t MapperSxROM ::readPRG(std::uint16_t addr)
{
    if (addr < 0xc000)
    {
        /* code */
        return *(m_firstBankPRG + (addr & 0x3fff));
    }
    else
    {
        return *(m_secondBankPRG + (addr & 0x3fff));
    }
}

void MapperSxROM::writePRG(std::uint16_t addr, std::uint8_t value)
{
    if (!(value & 0x80))
    {
        /* code */
        m_tempRegister = (m_tempRegister >> 1) | ((value & 1) << 4);
        ++m_writeCounter;
        if (m_writeCounter == 5)
        {
            /* code */
            if (addr <= 0x9fff)
            {
                /* code */
                switch (m_tempRegister & 0x3)
                {
                case 0:
                    /* code */
                    m_mirroing = OneScreenLower;
                    break;
                case 1:
                    /* code */
                    m_mirroing = OneScreenHighter;
                    break;
                case 2:
                    /* code */
                    m_mirroing = Vertical;
                    break;
                case 3:
                    /* code */
                    m_mirroing = Horizontal;
                    break;
                default:
                    break;
                }

                m_mirroringCallback();

                m_modeCHR = (m_tempRegister & 0x10) >> 4;
                m_modePRG = (m_tempRegister & 0xc) >> 2;

                calculatePRGPointers();

                if (m_modeCHR == 0) // one 8k bank
                {
                    /* code */
                    m_firstBankCHR = &m_cartridge.getVROM()[0x1000 * (m_regCHR0 | 1)];
                    m_secondBankCHR = m_firstBankCHR + 0x1000;
                }
                else // tow 4k banks
                {
                    m_firstBankCHR = &m_cartridge.getVROM()[0x1000 * m_regCHR0];
                    m_secondBankCHR = &m_cartridge.getVROM()[0x1000 * m_regCHR1];
                }
            }
            else if (addr <= 0xbfff)
            {
                /* code */
                m_regCHR0 = m_tempRegister;
                m_firstBankCHR = &m_cartridge.getVROM()[0x1000 * (m_tempRegister | (1 - m_modeCHR))];
                if (m_modeCHR == 0)
                {
                    /* code */
                    m_secondBankCHR = m_firstBankCHR + 0x1000;
                }
            }
            else if (addr <= 0xdfff)
            {
                /* code */
                m_regCHR1 = m_tempRegister;
                if (m_modeCHR == 1)
                {
                    /* code */
                    m_secondBankCHR = &m_cartridge.getVROM()[0x1000 * m_tempRegister];
                }
            }
            else
            {
                if ((m_tempRegister & 0x10) == 0x10)
                {
                    /* code */
                    LOG(Info) << "PRG-RAM activated" << std::endl;
                }

                m_tempRegister &= 0xf;
                m_regPRG = m_tempRegister;
                calculatePRGPointers();
            }

            m_tempRegister = 0;
            m_writeCounter = 0;
        }
        else
        {
            m_tempRegister = 0;
            m_writeCounter = 0;
            m_modePRG = 3;
            calculatePRGPointers();
        }
    }
}

std::uint8_t MapperSxROM ::readCHR(std::uint16_t addr)
{
    if (m_usesCharacterRAM)
    {
        /* code */
        return m_characterRAM[addr];
    }
    else if (addr < 0x1000)
    {
        /* code */
        return *(m_firstBankCHR + addr);
    }
    else
    {
        return *(m_secondBankCHR + (addr & 0xfff));
    }
}

void MapperSxROM::writeCHR(std::uint16_t addr, std::uint8_t value)
{
    if (m_usesCharacterRAM)
    {
        /* code */
        m_characterRAM[addr] = value;
    }
    else
    {
        LOG(Info) << "read-only chr memory write attempt at" << std::hex << addr << std::endl;
    }
}

NameTableMirroring MapperSxROM::getNameTableMirroring()
{
    return m_mirroing;
}

void MapperSxROM::calculatePRGPointers()
{
    if (m_modePRG <= 1)
    {
        m_firstBankPRG = &m_cartridge.getROM()[0x4000 * (m_regPRG & ~1)];
        m_secondBankPRG = m_firstBankPRG + 0x4000;
    }
    else if (m_modePRG == 2)
    {
        m_firstBankPRG = &m_cartridge.getROM()[0];
        m_secondBankPRG = m_firstBankPRG + 0x4000 * m_regPRG;
    }
    else
    {
        m_firstBankPRG = &m_cartridge.getROM()[0x4000 * m_regPRG];
        m_secondBankPRG = &m_cartridge.getROM()[m_cartridge.getROM().size() - 0x4000];
    }
}