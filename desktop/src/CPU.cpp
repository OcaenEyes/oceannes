/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-14 19:05:48
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-03-17 19:38:11
 * @FilePath: /oceannes/desktop/src/CPU.cpp
 * @Description: 注释信息
 */
#include "CPU.h"
#include "LogUtil.h"
#include "CPUOpcodes.h"
#include <iomanip>

CPU::CPU(MainBus &mem) : m_pendingNMI(false), m_pendingIRQ(false), m_bus(mem)
{
}

CPU::~CPU()
{
}

void CPU::reset()
{
    reset(readAddress(ResetVector));
}

void CPU::reset(std::uint16_t start_addr)
{
    m_skipCycles = m_cycles = 0;
    r_A = r_X = r_Y = 0;
    f_I = true;
    f_C = f_D = f_N = f_V = f_Z = false;
    r_PC = start_addr;
    r_SP = 0xfd;
}

void CPU::interrupt(InterruptType type)
{
    switch (type)
    {
    case InterruptType::NMI:
        /* code */
        m_pendingNMI = true;
        break;
    case InterruptType::IRQ:
        /* code */
        m_pendingIRQ = true;
        break;
    default:
        break;
    }
}

void CPU::interruptSequence(InterruptType type)
{
    if (f_I && type != NMI && type != BRK_)
    {
        /* code */
        return;
    }

    if (type == BRK_)
    {
        /* code */
        ++r_PC;
    }

    pushStack(r_PC >> 8);
    pushStack(r_PC);

    std::uint8_t flags = f_N << 7 |
                         f_V << 6 |
                         1 << 5 |

                         (type == BRK_) << 4 |
                         f_D << 3 |
                         f_I << 2 |
                         f_Z << 1 |
                         f_C;
    pushStack(flags);

    f_I = true;
    switch (type)
    {
    case IRQ:
        /* code */
        break;
    case BRK_:
        r_PC = readAddress(IRQVector);
        break;
    case NMI:
        r_PC = readAddress(NMIVector);
        break;
    default:
        break;
    }

    m_skipCycles += 6;
}

void CPU::pushStack(std::uint8_t value)
{
    m_bus.write(0x100 | r_SP, value);
    --r_SP;
}

std::uint8_t CPU::pullStack()
{
    return m_bus.read(0x100 | ++r_SP);
}

void CPU::setZN(std::uint8_t value)
{
    f_Z = !value;
    f_N = value & 0x80;
}

void CPU::setPageCrossed(std::uint16_t a, std::uint16_t b, int inc){
    
}
