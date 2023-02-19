/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-18 09:53:49
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-18 22:32:53
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

void MainBus::write(std::uint16_t addr, std::uint8_t value)
{
    if (addr < 0x2000)
    {
        /* code */
        m_RAM[addr & 0x7ff] = value;
    }
    else if (addr < 0x4020)
    {
        if (addr < 0x4000) // PPU 注册， mirrored
        {
            /* code */
            auto it = m_writeCallbacks.find(static_cast<IORegisters>(addr & 0x2007));
            if (it != m_writeCallbacks.end())
            {
                /* code */
                (it->second)(value);
            }
            else
            {
                LOG(InfoVerbose) << "no write callback registered for I/O register at : " << std::hex << +addr << std::endl;
            }
        }
        else if (addr < 0x4017 & addr >= 0x4014)
        {
            /* code */
            auto it = m_writeCallbacks.find(static_cast<IORegisters>(addr));
            if (it != m_writeCallbacks.end())
            {
                /* code */
                (it->second)(value);
            }
            else
            {
                LOG(InfoVerbose) << "no write callback registered for I/O register at : " << std::hex << +addr << std::endl;
            }
        }
        else
        {
            LOG(InfoVerbose) << "write access attmept at:" << std::hex << +addr << std::endl;
        }
    }
    else if (addr < 0x6000)
    {
        /* code */
        LOG(InfoVerbose) << "expansion rom access attempted. this is currently unsuported" << std::endl;
    }

    else if (addr < 0x8000)
    {
        /* code */
        if (m_mapper->hasExtenedRAM())
        {
            /* code */
            m_extRAM[addr - 0x6000] = value;
        }
    }
    else
    {
        m_mapper->writePRG(addr, value);
    }
}

const std::uint8_t *MainBus::getPagePtr(std::uint8_t page)
{
    std::uint16_t addr = page << 8;
    if (addr < 0x2000)
    {
        /* code */
        return &m_RAM[addr & 0x7fff];
    }
    else if (addr < 0x4020)
    {
        /* code */
        LOG(Error) << "register address memory pointer access attempt" << std::endl;
    }
    else if (addr < 0x6000)
    {
        /* code */
        LOG(Error) << "expansion rom access attempted ,which is unsupported" << std::endl;
    }
    else if (addr < 0x8000)
    {
        /* code */
        if (m_mapper->hasExtenedRAM())
        {
            /* code */
            return &m_extRAM[addr - 0x6000];
        }
    }
    else
    {
        LOG(Error) << "unexcepted dma request:" << std::hex << "0x" << +addr << "(" << +page << ")" << std::dec << std::endl;
    }
    return nullptr;
}

bool MainBus::setMapper(Mapper *mapper)
{
    m_mapper = mapper;
    if (!mapper)
    {
        /* code */
        LOG(Error) << "mapper pointer is nullptr" << std::endl;
        return false;
    }

    if (mapper->hasExtenedRAM())
    {
        /* code */
        m_extRAM.resize(0x2000);
    }

    return true;
}

bool MainBus::setWriteCallback(IORegisters reg, std::function<void(std::uint8_t)> callback)
{
    if (!callback)
    {
        /* code */
        LOG(Error) << "callback argument is nullptr" << std::endl;
        return false;
    }
    return m_writeCallbacks.emplace(reg, callback).second;
}

bool MainBus::setReadCallback(IORegisters reg, std::function<std::uint8_t(void)> callback)
{
    if (!callback)
    {
        /* code */
        LOG(Error) << "callback argument is nullptr" << std::endl;
        return false;
    }
    return m_readCallbacks.emplace(reg, callback).second;
}