/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-14 19:05:48
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-03-24 13:57:22
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

void CPU::setPageCrossed(std::uint16_t a, std::uint16_t b, int inc)
{
    m_skipCycles += 513;
    m_skipCycles += (m_cycles & 1);
}

void CPU::step()
{
    ++m_cycles;
    if (m_skipCycles-- > 1)
    {
        /* code */
        return;
    }

    m_skipCycles = 0;

    if (m_pendingNMI)
    {
        /* code */
        interruptSequence(NMI);
        m_pendingNMI = m_pendingIRQ = false;
        return;
    }
    else if (m_pendingIRQ)
    {
        interruptSequence(IRQ);
        m_pendingNMI = m_pendingIRQ = false;
        return;
    }

    int psw = f_N << 7 | f_V << 6 | 1 << 5 | f_D << 3 | f_I << 2 | f_Z << 1 | f_C;

    LOG_CPU << std::hex << std::setfill('0') << std::uppercase
            << std::setw(4) << +r_PC
            << "  "
            << "A:" << std::setw(2) << +r_A << " "
            << "X:" << std::setw(2) << +r_X << " "
            << "Y:" << std::setw(2) << +r_Y << " "
            << "P:" << std::setw(2) << +psw << " "
            << "SP:" << std::setw(2) << +r_SP << " "
            << "CYC:" << std::setw(3) << std::setfill(' ') << std::dec << ((m_cycles - 1) * 3) % 341
            << std::endl;

    std::uint8_t opcode = m_bus.read(r_PC++);
    auto CycleLength = OperationCycles[opcode];

    if (CycleLength && (executeImplied(opcode) || executeBranch(opcode) || executeType0(opcode) || executeType1(opcode) || executeType2(opcode)))
    {
        /* code */
        m_skipCycles += CycleLength;
    }
    else
    {
        LOG(Error) << "unrecognized opcode" << std::hex << +opcode << std::endl;
    }
}

bool CPU::executeImplied(std::uint8_t opcode)
{
    switch (static_cast<OperationImplied>(opcode))
    {
    case NOP:
        /* code */
        break;
    case BRK:
        interruptSequence(BRK_);
        break;
    case JSR:
        pushStack(static_cast<std::uint8_t>((r_PC + 1) >> 8));
        pushStack(static_cast<std::uint8_t>(r_PC + 1));
        r_PC = readAddress(r_PC);
        break;
    case RTS:
        r_PC = pullStack();
        r_PC |= pullStack() << 8;
        ++r_PC;
        break;
    case RTI:
        break;
    case JMP:
        break;
    case JMPI:
    {
    }
    break;
    case PHP:
        break;

    case PLP:
        break;

    case PHA:
        break;

    case PLA:
        break;

    case DEY:
        break;

    case DEX:
        break;

    case TAY:
        break;

    case INY:
        break;

    case INX:
        break;

    case CLC:
        break;

    case SEC:
        break;

    case CLI:
        break;

    case SEI:
        break;

    case CLD:
        break;

    case SED:
        break;

    case TYA:
        break;

    case CLV:
        break;

    case TXA:
        break;

    case TXS:
        break;
        
    
    case TAX:
        break;

    case TSX:
        break;

    default:
        break;
    }
}

bool CPU::executeBranch(std::uint8_t opcode)
{
}

bool CPU::executeType0(std::uint8_t opcode)
{
}

bool CPU::executeType1(std::uint8_t opcode)
{
}

bool CPU::executeType2(std::uint8_t opcode)
{
}