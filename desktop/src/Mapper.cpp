/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 00:43:57
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-19 12:04:31
 * @FilePath: /oceannes/desktop/src/Mapper.cpp
 * @Description: 注释信息
 */
#include "Mapper.h"
#include "CPUOpcodes.h"
#include "MapperAxROM.h"
#include "MapperCNROM.h"
#include "MapperGxROM.h"
#include "MapperMMC3.h"
#include "MapperNROM.h"
#include "MapperSxROM.h"
#include "MapperUxROM.h"
#include "MapperColorDreams.h"

Mapper::~Mapper()
{
}

NameTableMirroring Mapper::getNameTableMirroring()
{
    return static_cast<NameTableMirroring>(m_cartridge.getNameTableMirroring());
}

std::unique_ptr<Mapper> Mapper::createMapper(Mapper::Type mapper_t, Cartridge &cart, std::function<void()> intterupt_cb, std::function<void(void)> mirroring_cb)
{
}