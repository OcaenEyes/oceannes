/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:26:09
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-28 11:56:04
 * @FilePath: /vdesktop/include/Cartridge.h
 * @Description: 注释信息
 */
#pragma once

#include <vector>
#include <string>

#include "Chip.h"

/**
 * cartridge: 卡带
 * 卡带中 NES 文件格式：
 * |Header|Trainer  |PGR   |CHR  |
 * |16B   |0或者512B |n*16KB|n*8KB|
 * Header 中 Trainer 的 flag 为 1，则此区域为 512 字节，否则为 0
 * 模拟可不考虑 Trainer 段
 *
 */

class Cartridge
{
    public:
        Cartridge();
        bool LoadFromFile(std::string path);
        const std::vector<Byte>& GetROM();
        const std::vector<Byte>& GetVROM();
        bool HasExtendedRAM();
        Byte GetMapper();
        Byte GetNameTableMirroring();

    private:
        std::vector<Byte> m_PRG_ROM;   // 存放 NES 程序数据
        std::vector<Byte> m_CHR_ROM;   // 角色只读存储器，用于图像显示

        Byte m_nameTableMirroring;   
        Byte m_mapperNumber;            // mappper号 最基础为0
        bool m_extendedRAM;             // 卡带中是否存在扩展RAM
};
