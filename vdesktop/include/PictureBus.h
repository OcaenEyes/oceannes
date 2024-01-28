/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:25:38
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-28 12:02:13
 * @FilePath: /vdesktop/include/PictureBus.h
 * @Description: 注释信息
 */
#pragma once

#include "Cartridge.h"
#include "Mapper.h"

class PictureBus
{
public:
    PictureBus();
    Byte Read(Address addr);
    void Write(Address addr, Byte value);
    void UpdateMirroring();
    bool SetMapper(Mapper *mapper);
    /* 读调色板 */
    Byte ReadPalette(Byte paletteAddr);

private:
    std::vector<Byte> m_RAM;
    /*名称表，用来排列显示背景 */
    // std::size_t is the type of any sizeof expression and as is guaranteed to be able to express the maximum size of any object
    // indices where they start in RAM vector
    std::size_t m_NameTable0, m_NameTable1, m_NameTable2, m_NameTable3;

    std::vector<Byte> m_palette;

    Mapper *m_mapper;
};
