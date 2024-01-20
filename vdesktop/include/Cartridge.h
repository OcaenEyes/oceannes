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
private:
    std::vector<Byte> m_PRG_ROM; // 存放NES 程序数据
    std::vector<Byte> m_CHR_ROM; // 角色只读存储器， 用于图像显示

    Byte m_name_TableMirroring;
    Byte m_mapper_number; // mapper号，  最基础为0
    bool m_extended_RAM;  // 卡带中是否存在扩展RAM

public:
    Cartridge(/* args */);
    ~Cartridge();

    bool LoadFromFile(std::string path);
    const std::vector<Byte> &GetRom();
    const std::vector<Byte> &GetVRom();

    bool HasExtendedRAM();
    Byte GetMapper();
    Byte GetNameTableMirroring();
};
