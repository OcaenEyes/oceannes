/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:29:35
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-20 05:15:49
 * @FilePath: /vdesktop/src/Mapper.cc
 * @Description: 注释信息
 */
#include "Mapper.h"

Mapper::Mapper(Cartridge &cart, MapperType t) : m_cartridge(cart), m_type(t)
{
}

NameTableMirroring Mapper::GetNameTableMirroring()
{
    return static_cast<NameTableMirroring>(m_cartridge.GetNameTableMirroring());
}

inline bool Mapper::HasExtendedRAM()
{
    return m_cartridge.HasExtendedRAM();
}

std::unique_ptr<Mapper> Mapper::CreateMapper(MapperType t, Cartridge &cart, std::function<void(void)> mirroring_cb)
{
    return std::unique_ptr<Mapper>();
}
