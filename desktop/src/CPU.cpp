/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-14 19:05:48
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-18 10:14:57
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
