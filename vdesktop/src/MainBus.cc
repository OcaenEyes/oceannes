/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:29:29
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-20 16:29:24
 * @FilePath: \vdesktop\src\MainBus.cc
 * @Description: 注释信息
 */
#include "MainBus.h"
#include "Log.h"

// 0x800 = 2KB
MainBus::MainBus(/* args */) : m_RAM(0x800, 0)
{
}

MainBus::~MainBus()
{
}

bool MainBus::SetMapper(Mapper *mapper)
{
    if (mapper == nullptr)
    {
        LOG(ERROR) << "Mapper point is nullptr\n";
        return false;
    }
    m_mapper = mapper;
    if (mapper->HasExtendedRAM())
    {
        m_extended_RAM.resize(0x2000);
    }
    return true;
}

/**
 * CPU Memory Map
 * -------------------------------- $10000
 * Upper Bank of Cartridge ROM
 * -------------------------------- $C000
 * Lower Bank of Cartridge ROM
 * -------------------------------- $8000
 * Cartridge RAM (may be battery-backed)
 * -------------------------------- $6000
 * Expansion Modules
 * -------------------------------- $5000
 * Input/Output
 * -------------------------------- $2000
 * 2KB Internal RAM, mirrored 4 times
 * -------------------------------- $0000
 *
 */

/**
 * @brief
 *
 * @param addr
 * @return Byte
 */
Byte MainBus::Read(Address addr)
{
    // 0x2000 = 8KB RAM
    if (addr < 0x2000)
    {
        // 实际只有2KB的 RAM（如上定义的0x800）, 所以采用 addr & 0x7ff的操作
        return m_RAM[addr & 0x7ff];
    }
    else if (addr < 0x4020)
    {
        // PPU 寄存器 映射到了总线上
        // 位于 $2000-$2007 , 另一个寄存器用于直接内存访问， 定制为$4014
        // 位置$2000-$2007 在 $2008-$3FFF区域中，每8个字节镜像一次
        if (addr < 0x4000)
        {
            // IO 寄存器的读写使用map来实现
            auto it = m_read_callbacks.find(static_cast<IORegisters>(addr & 0x2007));
            if (it != m_read_callbacks.end())
            {
                // it->second 保存了Read函数的对象
                return (it->second)();
            }
            else
            {
                LOG(INFO) << "No read callback registered for I/O register at:" << std::hex << +addr << "\n";
            }
        }
        else if (addr < 0x4018 && addr >= 0x4014)
        {
            auto it = m_read_callbacks.find(static_cast<IORegisters>(addr));
            if (it != m_read_callbacks.end())
            {
                // it->second 保存了Read函数的对象
                return (it->second)();
            }
            else
            {
                LOG(INFO) << "No read callback registered for I/O register at:" << std::hex << +addr << "\n";
            }
        }
        else
        {
            LOG(INFO) << "Read access attempt at:" << std::hex << +addr << "\n";
        }
    }
    else if (addr < 0x6000)
    {
        LOG(INFO) << "Expansion ROM access attempted, which is unsupported.\n";
    }
    else if (addr < 0x8000)
    {
        if (m_mapper->HasExtendedRAM())
        {
            return m_extended_RAM[addr - 0x6000];
        }
    }
    else
    {
        return m_mapper->ReadPRG(addr);
    }

    return 0;
}

/**
 * @brief
 *
 * @param addr
 * @param value
 */
void MainBus::Write(Address addr, Byte value)
{
    if (addr < 0x2000)
    {
        m_extended_RAM[addr & 0x7ff] = value;
    }
    else if (addr < 0x4020)
    {
        if (addr < 0x4000) // PPU registers ,mirrored
        {
            auto it = m_write_callbacks.find(static_cast<IORegisters>(addr & 0x2007));
            if (it != m_write_callbacks.end())
            {
                (it->second)(value);
            }
            else
            {
                LOG(INFO) << "No write callback registered for I/O register at:" << std::hex << +addr << "\n";
            }
        }
        else if (addr < 0x4017 && addr >= 0x4014)
        {
            auto it = m_write_callbacks.find(static_cast<IORegisters>(addr));
            if (it != m_write_callbacks.end())
            {
                (it->second)(value);
            }
            else
            {
                LOG(INFO) << "No write callback registered for I/O register at:" << std::hex << +addr << "\n";
            }
        }
        else
        {
            LOG(INFO) << "Write access attempt at:" << std::hex << +addr << "\n";
        }
    }
    else if (addr < 0x6000)
    {
        LOG(INFO) << "Expansion ROM access attempted, which is unsupported.\n";
    }
    else if (addr < 0x8000)
    {
        if (m_mapper->HasExtendedRAM())
        {
            m_extended_RAM[addr - 0x6000] = value;
        }
    }
    else
    {
        m_mapper->WritePRG(addr, value);
    }
}

/**
 * @brief
 *  DMA 的访问操作以页为单位
 *  每个页大小256B
 * @param page
 * @return const Byte*
 */
const Byte *MainBus::GetPagePtr(Byte page)
{
    Address addr = page << 8;
    if (addr < 0x2000)
    {
        return &m_RAM[addr & 0x7fff];
    }
    else if (addr < 0x4020)
    {
        LOG(ERROR) << "Register address memory pointer access attempt.\n";
    }
    else if (addr < 0x6000)
    {
        LOG(ERROR) << "Expansion ROM access attempted, which is unsupported.\n";
    }
    else if (addr < 0x8000)
    {
        if (m_mapper->HasExtendedRAM())
        {
            return &m_extended_RAM[addr - 0x6000];
        }
    }
    return nullptr;
}

bool MainBus::SetWriteCallback(IORegisters reg, std::function<void(Byte)> callback)
{
    if (!callback)
    {
        LOG(ERROR) << "write callback argument is nullptr\n";
        return false;
    }
    return m_write_callbacks.emplace(reg, callback).second;
}

bool MainBus::SetReadCallback(IORegisters reg, std::function<Byte(void)> callback)
{
    if (!callback)
    {
        LOG(ERROR) << "read callback argument is nullptr\n";
        return false;
    }
    return m_read_callbacks.emplace(reg, callback).second;
}
