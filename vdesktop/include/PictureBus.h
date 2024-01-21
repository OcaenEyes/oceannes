/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:25:38
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-20 19:11:10
 * @FilePath: /vdesktop/include/PictureBus.h
 * @Description: 注释信息
 */
#pragma once

#include "Cartridge.h"
#include "Mapper.h"

class PictureBus
{
private:
    std::vector<Byte> m_RAM;
    // 名称表 用于排列显示背景
    std::size_t m_name_table0, m_name_table1, m_name_table2, m_name_table3;

    std::vector<Byte> m_palette;
    Mapper *m_mapper;

public:
    PictureBus(/* args */);
    ~PictureBus();

    Byte Read(Address addr);
    void Write(Address addr, Byte value);
    void UpdateMirroring();
    bool SetMapper(Mapper *mapper);

    // 读取调色板
    Byte ReadPalette(Byte palette_addr);
};
