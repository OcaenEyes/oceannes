/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:25:09
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-20 05:13:27
 * @FilePath: /vdesktop/include/Mapper.h
 * @Description: 注释信息
 */
#pragma once

#include <memory>
#include <functional>

#include "Cartridge.h"

enum NameTableMirroring
{
    Horizontal = 0, // 横
    Vertical = 1,   // 竖
    FourScreen = 8,
    OneScreenLower,
    OneScreenHigher
};

/**
 * mapper，这个概念来源于 memory mapping，又叫做 Memory Management Chip，它是解决地址映射的一种电路，简单来说就是决定物理内存如何映射到 CPU 或者 PPU 的地址空间。
 * mapper 可以用来支持增加卡带的 RAM 甚至支持额外的音频通道，但更一般的目的就是控制物理内存到地址空间的映射，突破游戏 40KB 的限制。
 * 为什么说是 40KB 的限制，因为早期一般的游戏最大就是的2 * 16KB =32KB的PRG，以及 8KB 的CHR，加起来就是 40KB，而更复杂的 mapper 硬件可以使得游戏突破这个限制。
 *
 * NROM
 *  mapper 000，排在第零个，最简单的一种 mapper，像超级马里奥就使用的是 NROM。NROM 也分种类，NROM-128 的存放 PRG 的 ROM(后面简称 PRG-ROM) 只有 16KB，而 NROM-256 有 256KB。
 *  PRG 的第一个 bank(16KB) 映射到 0x8000-0xBFFF，最后一个 bank 映射到 0xC000-0xFFFF。
 *  PPU 的 VRAM 开头 8KB 映射到 CHR，也就是 PatternTable
 *
 *
 * MMC1
 *  mapper 001，使用 MMC1 的游戏有双截龙，恶魔城等，来看其 bank 和映射关系：
 *  CPU 0x6000-0x7FFF 映射到 8KB 的 PRG RAM bank，这是可选的
 *  CPU 0x8000-0xBFFF 映射到 PRG ROM 的一个 bank，这个 bank 要么是可切换的要么固定为第一个
 *  CPU 0xC000-0xFFFF 映射到 PRG ROM 的一个 bank，这个 bank 要么是可切换的要么固定为最后一个
 *  PPU 0x0000-0x0FFF 映射到一个 4KB 可切换的 CHR bank
 *  PPU 0x1000-0x1FFF 映射到一个 4KB 可切换的 CHR bank
 *
 * UNROM
 *  mapper 002，使用这个 mapper 的著名游戏有魂斗罗，洛克人等等。UNROM 比较厉害，最高可支持 4M 的 PRG，要知道 M 这个单位在那时对游戏来说是个很大的单位了。虽然 UNROM 可以支持 4M 的大容量 PRG，但其实 UNROM 还没 MMC1 复杂，来看其 banks 的规划(映射关系)
 *  CPU 0x8000-0xBFFF，16KB 可切换的 PRG ROM bank
 *  CPU 0xCFFF-0xFFFF，16KB 固定的 PRG ROM bank，这部分地址空间固定映射到最后一块 PRG bank
 *  CHR 容量 8KB，就映射到 PPU 开头的 2KB
 *
 * CNROM
 *  mapper003 CNROM，较为出名的游戏有勇者斗恶龙，高桥名人的冒险岛等等
 *
 */
class Mapper
{
public:
    enum MapperType
    {
        NROM, //  mapper 0  对应最直观的ROM管理方式
        SxROM,
        UxROM,
        CNROM
    };

    Mapper(Cartridge &cart, MapperType t);
    virtual ~Mapper() = default;

    virtual void WritePRG(Address addr, Byte value) = 0;
    virtual Byte ReadPRG(Address addr) = 0;

    virtual void WriteCHR(Address addr, Byte value) = 0;
    virtual Byte ReadCHR(Address addr) = 0;

    virtual NameTableMirroring GetNameTableMirroring();

    bool inline HasExtendedRAM();

    static std::unique_ptr<Mapper> CreateMapper(MapperType t, Cartridge &cart, std::function<void(void)> mirroring_cb = nullptr);

protected:
    Cartridge &m_cartridge;
    MapperType m_type;
};
