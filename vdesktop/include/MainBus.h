/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:12:09
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-20 14:09:00
 * @FilePath: \vdesktop\include\MainBus.h
 * @Description: 注释信息
 */
#pragma once

#include "Mapper.h"
#include "Cartridge.h"

#include <map>
#include <vector>

/*
 * IO寄存器的具体含义参考 http://fms.komkon.org/EMUL8/NES.html#LABF
 */

enum IORegisters
{
    PPUCTL = 0x2000, // 控制寄存器
    PPUMASK,
    PPUSTATUS, // 状态寄存器
    OAMADDR,   // Sprite 内存地址寄存器
    OAMDATA,   // Sprite Memory Data
    PPUSCROL,  // Screen Scroll Offsets 屏幕滚动偏移
    PPUADDR,   // PPU Memory Address
    PPUDATA,   // PPU Memory Data

    // 中间空余的为 Sound寄存器
    OAMDMA = 0x4014, // DMA Access to the Sprite Memory
    JOY1 = 0x4016,   // Joystick 1 + Strobe
    JOY2 = 0x4017,   // Joystick 2 + Strobe
};

class MainBus
{
private:
    std::vector<Byte> m_RAM;          // 使用vector 保存内存信息
    std::vector<Byte> m_extended_RAM; // 扩展RAM

    Mapper *m_mapper;
    std::map<IORegisters, std::function<void(Byte)>> m_write_callbacks;
    std::map<IORegisters, std::function<Byte(void)>> m_read_callbacks;

public:
    MainBus(/* args */);
    ~MainBus();

    bool SetMapper(Mapper *mapper);

    Byte Read(Address addr);
    void Write(Address addr, Byte value);

    const Byte *GetPagePtr(Byte page);

    // IO 寄存器读写
    //  提供一个抽象接口，交由对应的硬件模块提供函数获得相应的寄存器值
    bool SetWriteCallback(IORegisters reg, std::function<void(Byte)> callback);
    bool SetReadCallback(IORegisters reg, std::function<Byte(void)> callback);
};
