/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:28:34
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-21 14:26:01
 * @FilePath: \vdesktop\src\CPU.cc
 * @Description: 注释信息
 */
#include "CPU.h"
#include "Cartridge.h"
#include "Log.h"
#include "CPUOpcodes.h"

/*
参考：中文版 http://nesdev.com/nes_c.txt
     英文版： http://fms.komkon.org/EMUL8/NES.html
CPU Memory Map
--------------------------------------- $10000
 Upper Bank of Cartridge ROM            卡带的上层ROM
--------------------------------------- $C000
 Lower Bank of Cartridge ROM            卡带的下层ROM
--------------------------------------- $8000
 Cartridge RAM (may be battery-backed)  卡带的RAM（可能有电池支持）
--------------------------------------- $6000
 Expansion Modules                      扩充的模块
--------------------------------------- $5000
 Input/Output                           输入/输出
--------------------------------------- $2000
 2kB Internal RAM, mirrored 4 times     2KB的内部RAM，做4次镜象
--------------------------------------- $0000
*/

CPU::CPU(MainBus &mem) : m_bus(mem)
{
}

void CPU::Reset()
{
    Reset(ReadAddress(ResetVector));
}

void CPU::Reset(Address start_addr)
{
    m_skip_cycles = m_cylces = 0;
    r_A = r_X = r_Y = 0;
    f_I = true;

    f_C = f_D = f_N = f_V = f_Z = false;
    r_PC = start_addr;

    r_SP = 0xfd;
}

void CPU::SkipDMACycles()
{
    m_skip_cycles += 513;            // 256 read + 256 write + 1 dummy read
    m_skip_cycles += (m_cylces & 1); // +1 if on odd cycle
}

void CPU::Step()
{
    ++m_cylces;
    if (m_skip_cycles-- > 1)
    {
        return;
    }

    m_skip_cycles = 0;
    // 生成程序状态字
    Byte opcode = m_bus.Read(r_PC++);
    auto CycleLength = OperationCycles[opcode];

    if (CycleLength && (ExecuteImplied(opcode) || ExecuteBranch(opcode) ||
                        ExecuteType0(opcode) || ExecuteType1(opcode) || ExecuteType2(opcode)))
    {
        m_skip_cycles += CycleLength;
    }
}

Address CPU::GetPC()
{
    return r_PC;
}

void CPU::Interrupt(InterruptType t)
{
    if (f_I && t != NMI && t != BRK_)
    {
        return;
    }

    if (t == BRK_) // add one if BRK , a quirk of 6502
    {
        ++r_PC;
    }

    // 保存PC 值
    PushStack(r_PC >> 8);
    PushStack(r_PC);

    Byte flags = f_N << 7 | f_V << 6 | 1 << 5 // unused bit , supposed to be always 1
                 | (t == BRK_) << 4 |         // B flag set if BRK
                 f_D << 3 |
                 f_I << 2 |
                 f_Z << 1 | f_C;

                 // 保存状态
                 PushStack(flags);
}

// 内存addr处， 保持着一个地址信息
Address CPU::ReadAddress(Address addr)
{
    // （1）移位运算符在乘除加减后面，在比较运算符前面
    // （2）按位与、或、异或在比较运算符后面，在逻辑与、或前面
    return m_bus.Read(addr) | m_bus.Read(addr + 1) << 8;
}

bool CPU::ExecuteImplied(Byte opcode)
{
    return false;
}

bool CPU::ExecuteBranch(Byte opcode)
{
    return false;
}

bool CPU::ExecuteType0(Byte opcode)
{
    return false;
}

bool CPU::ExecuteType1(Byte opcode)
{
    return false;
}

bool CPU::ExecuteType2(Byte opcode)
{
    return false;
}

void CPU::SetPageCrossed(Address a, Address b, int inc)
{
    //  Page is determined by the high byte
    if ((a & 0xff00) != (b & 0xff00))
    {
        m_skip_cycles += inc;
    }
}

// 从内存的栈中读/写数据
void CPU::PushStack(Byte value)
{
    m_bus.Write(0x100 | r_SP, value);
    --r_SP;
}

Byte CPU::PullStack()
{
    return m_bus.Read(0x100 | ++r_SP);
}

void CPU::SetZN(Byte value)
{
    // value 为0 则f_Z 置位
    f_Z = !value;
    f_N = value & 0x80;
}
