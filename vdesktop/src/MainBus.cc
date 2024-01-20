/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:29:29
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-20 14:19:23
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
        auto it = m_read_callbacks.find(static_cast<IORegisters>(addr & 0x2007));
        if (it != m_read_callbacks.end())
        {
            // it->second 保存了Read函数的对象
        }
        
    }
}

/**
 * @brief
 *
 * @param addr
 * @param value
 */
void MainBus::Write(Address addr, Byte value)
{
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
        LOG(ERROR) << "Register ROM access attempted, which is unsupported.\n";
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
