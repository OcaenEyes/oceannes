/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:28:34
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-21 15:46:38
 * @FilePath: /vdesktop/src/CPU.cc
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
    f_I = true;

    switch (t)
    {
    case IRQ:
    case BRK_:
        r_PC = ReadAddress(IRQVector);
        break;
    case NMI:
        r_PC = ReadAddress(NMIVector);
        break;
    }

    m_skip_cycles += 7;
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
    // static_cast 类型转换
    switch (static_cast<OperationImplied>(opcode))
    {
    case NOP:
        break;
    case JSR:
        /* jump to new location Saving Return Address */
        // Push address of next instruction - 1, thus r_PC + 1 instead of r_PC + 2
        // since r_PC and r_PC + 1 are address of subroutine
        PushStack(static_cast<Byte>((r_PC + 1) >> 8));
        PushStack(static_cast<Byte>(r_PC + 1));
        r_PC = ReadAddress(r_PC);
        break;
    case RTS:
        // return from subroutine
        r_PC = PullStack();
        r_PC |= PullStack() << 8;
        ++r_PC;
        break;
    case RTI:
    {
        Byte flags = PullStack();
        f_N = flags & 0x80;
        f_V = flags & 0x40;
        f_D = flags & 0x8;
        f_I = flags & 0x4;
        f_Z = flags & 0x2;
        f_C = flags & 0x1;
    }
        r_PC = PullStack();
        r_PC |= PullStack() << 8;
        break;
    case JMP:
        r_PC = ReadAddress(r_PC);
        break;
    case JMPI:
    {
        Address location = ReadAddress(r_PC);
        Address Page = location & 0xff00;
        r_PC = m_bus.Read(location) | m_bus.Read(Page | ((location + 1) & 0xff)) << 8;
    }
    break;
    case PHP:
    {
        Byte flags = f_N << 7 |
                     f_V << 6 |
                     1 << 5 | // supposed to always be 1
                     1 << 4 | // PHP pushes with the B flag as 1, no matter what
                     f_D << 3 |
                     f_I << 2 |
                     f_Z << 1 |
                     f_C;
        PushStack(flags);
    }
    break;
    case PLP:
    {
        Byte flags = PullStack();
        f_N = flags & 0x80;
        f_V = flags & 0x40;
        f_D = flags & 0x8;
        f_I = flags & 0x4;
        f_Z = flags & 0x2;
        f_C = flags & 0x1;
    }
    break;

    case PHA:
        PushStack(r_A);
        break;
    case PLA:
        r_A = PullStack();
        SetZN(r_A);
        break;
    case DEY:
        --r_Y;
        SetZN(r_Y);
        break;
    case DEX:
        --r_X;
        SetZN(r_X);
        break;
    case TAY:
        r_Y = r_A;
        SetZN(r_Y);
        break;
    case INY:
        ++r_Y;
        SetZN(r_Y);
        break;
    case INX:
        ++r_X;
        SetZN(r_X);
        break;
    case CLC:
        f_C = false;
        break;
    case SEC:
        f_C = true;
        break;
    case CLI:
        f_I = false;
        break;
    case SEI:
        f_I = true;
        break;
    case CLD:
        f_D = false;
        break;
    case SED:
        f_D = true;
        break;
    case TYA:
        r_A = r_Y;
        SetZN(r_A);
        break;
    case CLV:
        f_V = false;
        break;
    case TXA:
        r_A = r_X;
        SetZN(r_A);
        break;
    case TXS:
        r_SP = r_X;
        break;
    case TAX:
        r_X = r_A;
        SetZN(r_X);
        break;
    case TSX:
        r_X = r_SP;
        SetZN(r_X);
        break;

    default:
        return false;
    }
    return true;
}

bool CPU::ExecuteBranch(Byte opcode)
{
    // 跳转指令 实现
    if ((opcode & BranchConditionMask) == BranchInstructionMaskResult)
    {
        // branch is initialized to the condition required (for the flag specified later)
        bool branch = opcode & BranchConditionMask;
        
    }

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
