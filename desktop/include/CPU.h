/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-14 19:05:43
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-03-17 19:39:11
 * @FilePath: /oceannes/desktop/include/CPU.h
 * @Description: 注释信息
 */
#if !defined(CPU_H)
#define CPU_H

#include "CPUOpcodes.h"
#include "MainBus.h"

class CPU
{
private:
    /* data */
    void interruptSequence(InterruptType type);

    void executeImplied();
    void executeBranch();

    bool executeType0();
    bool executeType1();
    bool executeType2();

    std::uint16_t readAddress(std::uint16_t addr);

    void pushStack(std::uint8_t value);
    std::uint8_t pullStack();

    // If a and b are in different pages, increases the m_SkipCycles by inc
    void setPageCrossed(std::uint16_t a, std::uint16_t b, int inc);
    void setZN(std::uint8_t value);

    int m_skipCycles;
    int m_cycles;

    std::uint16_t r_PC;
    std::uint8_t r_SP;
    std::uint8_t r_A;
    std::uint8_t r_X;
    std::uint8_t r_Y;

    // 状态标记
    bool f_C;
    bool f_Z;
    bool f_I;
    bool f_D;
    bool f_V;
    bool f_N;

    bool m_pendingNMI;
    bool m_pendingIRQ;

    MainBus &m_bus;

public:
    CPU(MainBus &mem);

    void step();
    void reset();
    void reset(std::uint16_t start_addr);
    void log();

    std::uint16_t getPC();

    void skipDMACyles();

    void interrupt(InterruptType type);

    ~CPU();
};

#endif // CPU_H