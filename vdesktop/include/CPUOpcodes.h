/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:12:24
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-20 17:55:45
 * @FilePath: \vdesktop\include\CPUOpcodes.h
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