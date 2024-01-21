/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:12:24
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-21 14:48:29
 * @FilePath: /vdesktop/include/CPUOpcodes.h
 * @Description: 注释信息
 */
#pragma once

// 参考rockwell_r650x.pdf文件

const auto InstructionModeMask = 0x3;

const auto OperationMask = 0xe0;
const auto OperationShift = 5;

/**
 * 操作码的哪几位指定了寻址方式？
 *  datasheet中 16 * 16 的表格， 每一列的寻址方式是基本相同的
 *
 */

const auto AddrModeMask = 0x1c;
const auto AddrModeShift = 2;

const auto BranchInstructionMask = 0x1f; // 0001 --- 1111
const auto BranchInstructionMaskResult = 0x10;
const auto BranchConditionMask = 0x20;
const auto BranchFlagShift = 6;

// 6502 CPU 操作码相关定义
// 复位向量地址， 当CPU复位时， CPU将从此处地址开始取值执行
const auto ResetVector = 0xfffc;
const auto IRQVector = 0xfffe;

/* non-maskable interrupts */
/* When a VBlank interrupt occurs, CPU pushes return address and the status register on stack, then jumps to the address stored at location $FFFA */
const auto NMIVector = 0xfffa;

enum BranchOnFlag
{
    Negative,
    Overflow,
    Carry,
    Zero
};

// 指令集声明
enum Operation0
{
    BIT = 1, // Test BIT in memory with ACC
    STY = 4, // Store Index Y in memory
    LDY,
    CPY, // Compare Memory and Index Y
    CPX
};

enum Operation1
{
    ORA, // 'OR' memory with ACC
    AND,
    EOR, // 'Exclusive OR' with ACC
    ADC,
    STA,
    LDA, // Load ACC with Mem
    CMP,
    SBC
};

enum Operation2
{
    ASL, // Shift Left one Bit (Memory or ACC)
    ROL,
    LSR,
    ROR,
    STX,
    LDX,
    DEC,
    INC
};

/**
 * 隐含寻址的指令集编码
 * 参考指令集编码 Implied
 */
enum OperationImplied
{
    NOP = 0xea,
    
    JSR = 0x20,
    RTI = 0x40,
    RTS = 0x60,

    JMP = 0x4c,
    JMPI = 0x6c, // JMP Indirect

    PHP = 0x08,
    PLP = 0x28,
    PHA = 0x48,
    PLA = 0x68,

    DEY = 0x88,
    DEX = 0xca,
    TAY = 0xa8,
    INY = 0xc8,
    INX = 0xe8,

    CLC = 0x18,
    SEC = 0x38,
    CLI = 0x58,
    SEI = 0x78,
    TYA = 0x98,
    CLV = 0xb8,
    CLD = 0xd8,
    SED = 0xf8,

    TXA = 0x8a,
    TXS = 0x9a,
    TAX = 0xaa,
    TSX = 0xba,

};

enum AddressingMode1
{
    IndexedIndirectX,
    ZeroPage,
    Immediate, // 立即数寻址
    Absolute,  // 绝对值寻址
    IndirectY, // 寄存器间接寻址
    IndirectX, // 寄存器间接寻址
    AbsoluteY,
    AbsoluteX,
};

enum AddressingMode2
{
    Immediate_,
    ZeroPage_,
    Accumulator,
    Absolute_,
    Indexed = 5,
    AbsoluteIndexed = 7,
};

/*
 * 每条指令所需要的时钟数量
 * 0代表该指令编码没有使用
 */
int OperationCycles[0x100] = {
    7,
    6,
    0,
    0,
    0,
    3,
    5,
    0,
    3,
    2,
    2,
    0,
    0,
    4,
    6,
    0,
    2,
    5,
    0,
    0,
    0,
    4,
    6,
    0,
    2,
    4,
    0,
    0,
    0,
    4,
    7,
    0,
    6,
    6,
    0,
    0,
    3,
    3,
    5,
    0,
    4,
    2,
    2,
    0,
    4,
    4,
    6,
    0,
    2,
    5,
    0,
    0,
    0,
    4,
    6,
    0,
    2,
    4,
    0,
    0,
    0,
    4,
    7,
    0,
    6,
    6,
    0,
    0,
    0,
    3,
    5,
    0,
    3,
    2,
    2,
    0,
    3,
    4,
    6,
    0,
    2,
    5,
    0,
    0,
    0,
    4,
    6,
    0,
    2,
    4,
    0,
    0,
    0,
    4,
    7,
    0,
    6,
    6,
    0,
    0,
    0,
    3,
    5,
    0,
    4,
    2,
    2,
    0,
    5,
    4,
    6,
    0,
    2,
    5,
    0,
    0,
    0,
    4,
    6,
    0,
    2,
    4,
    0,
    0,
    0,
    4,
    7,
    0,
    0,
    6,
    0,
    0,
    3,
    3,
    3,
    0,
    2,
    0,
    2,
    0,
    4,
    4,
    4,
    0,
    2,
    6,
    0,
    0,
    4,
    4,
    4,
    0,
    2,
    5,
    2,
    0,
    0,
    5,
    0,
    0,
    2,
    6,
    2,
    0,
    3,
    3,
    3,
    0,
    2,
    2,
    2,
    0,
    4,
    4,
    4,
    0,
    2,
    5,
    0,
    0,
    4,
    4,
    4,
    0,
    2,
    4,
    2,
    0,
    4,
    4,
    4,
    0,
    2,
    6,
    0,
    0,
    3,
    3,
    5,
    0,
    2,
    2,
    2,
    0,
    4,
    4,
    6,
    0,
    2,
    5,
    0,
    0,
    0,
    4,
    6,
    0,
    2,
    4,
    0,
    0,
    0,
    4,
    7,
    0,
    2,
    6,
    0,
    0,
    3,
    3,
    5,
    0,
    2,
    2,
    2,
    2,
    4,
    4,
    6,
    0,
    2,
    5,
    0,
    0,
    0,
    4,
    6,
    0,
    2,
    4,
    0,
    0,
    0,
    4,
    7,
    0,
};