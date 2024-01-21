/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:30:23
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-21 23:15:26
 * @FilePath: \vdesktop\src\PPU.cc
 * @Description: 注释信息
 */
#include "PPU.h"
#include "Log.h"
#include "PaletteColors.h"

/*
 * PPU的实现是NES模拟器最复杂的一部分
 */

PPU::PPU(PictureBus &bus, VirtualScreen &screen) : m_bus(bus),
                                                   m_screen(screen),
                                                   m_sprite_memory(64 * 4),
                                                   m_picture_buffer(ScanlineVisibleDots, std::vector<sf::Color>(VisibleScanlines, sf::Color::Magenta))
{
    // VisibleScanlines 红白机 240
    // Magenta 品红
}

PPU::~PPU()
{
}

void PPU::Step()
{
}