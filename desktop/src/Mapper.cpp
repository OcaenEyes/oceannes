/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 00:43:57
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-20 00:16:22
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
    std::unique_ptr<Mapper> ret(nullptr);

    switch (mapper_t)
    {
    case NORM:
        /* code */
        ret.reset(new MapperNROM(cart));
        break;
    case SxROM:
        /* code */
        ret.reset(new MapperSxROM(cart, mirroring_cb));
        break;
    case UxROM:
        /* code */
        ret.reset(new MapperUxROM(cart));
        break;
    case CNROM:
        /* code */
        ret.reset(new MapperCNROM(cart));
        break;
    case MMC3:
        /* code */
        ret.reset(new MapperMMC3(cart, intterupt_cb, mirroring_cb));
        break;
    case AxROM:
        /* code */
        ret.reset(new MapperAxROM(cart, mirroring_cb));
        break;
    case GxROM:
        /* code */
        ret.reset(new MapperGxROM(cart, mirroring_cb));
        break;
    case ColorDreams:
        /* code */
        ret.reset(new MapperColorDreams(cart, mirroring_cb));
        break;

    default:
        break;
    }

    return ret;
}