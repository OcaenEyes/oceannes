/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-18 09:53:49
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-18 10:14:14
 * @FilePath: /oceannes/desktop/src/MainBus.cpp
 * @Description: 注释信息
 */
#include "MainBus.h"
#include "LogUtil.h"

MainBus::MainBus(/* args */) : m_RAM(0x800, 0), m_mapper(nullptr)
{
}

MainBus::~MainBus()
{
}

std::uint8_t MainBus::read(std::uint16_t addr)
{
    if (addr < 0x2000)
    {
        return m_RAM[addr & 0x7ff];
    }
    else if (addr < 0x4020)
    {
        if (addr < 0x4000)
        {
            auto it = m_readCallbacks.find(static_cast<IORegisters>(addr & 0x2007));
            if (it != m_readCallbacks.end())
            {
                return (it->second());
            }
            else
            {
                LOG(InfoVerbose) << "No read callback registered for I/O register at: " << std::hex << +addr << std::endl;
            }
        }
        else if (addr < 0x4018 && addr >= 0x4014)
        {
            auto it = m_readCallbacks.find(static_cast<IORegisters>(addr));

            if (it != m_readCallbacks.end())
            {
                return (it->second());
            }
            else
            {
                LOG(InfoVerbose) << "No read callback registered for I/O register at: " << std::hex << +addr << std::endl;
                ;
            }
        }
        else
        {
            LOG(InfoVerbose) << "Read access attempt at: " << std::hex << +addr << std::endl;
        }
    }
    else if (addr < 0x6000)
    {
        LOG(InfoVerbose) << "Expansion ROM read attempted. This is currently unsupported" << std::endl;
    }
    else if (addr < 0x8000)
    {
        if (m_mapper->hasExtenedRAM())
        {
            return m_extRAM[addr - 0x6000];
        }
    }
    else
    {
        return m_mapper->readPRG(addr);
    }
    return 0;
}