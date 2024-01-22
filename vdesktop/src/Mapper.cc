/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:29:35
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-22 15:30:53
 * @FilePath: /vdesktop/src/Mapper.cc
 * @Description: 注释信息
 */
#include "Mapper.h"
#include "MapperNROM.h"

Mapper::Mapper(Cartridge &cart, MapperType t) : m_cartridge(cart), m_type(t)
{
}

NameTableMirroring Mapper::GetNameTableMirroring()
{
    return static_cast<NameTableMirroring>(m_cartridge.GetNameTableMirroring());
}

bool Mapper::HasExtendedRAM()
{
    return m_cartridge.HasExtendedRAM();
}

std::unique_ptr<Mapper> Mapper::CreateMapper(MapperType t, Cartridge &cart, std::function<void(void)> mirroring_cb)
{
    std::unique_ptr<Mapper> ret(nullptr);
    switch (t)
    {
    case NROM:
        ret.reset(new MapperNROM(cart));
        break;
    case SxROM:
        break;
    case UxROM:
        break;
    case CNROM:
        break;
    default:
        break;
    }
    return ret;
}
