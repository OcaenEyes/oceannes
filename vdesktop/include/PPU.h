/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:25:14
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-21 23:10:24
 * @FilePath: \vdesktop\include\PPU.h
 * @Description: 注释信息
 */
#pragma once

#include "Chip.h"
#include "PictureBus.h"
#include "VirtualScreen.h"

#include <vector>

const int ScanlineCycleLength = 341;
const int ScanlineEndCycle = 340;

// NES图像分辨率 256 * 240(长*宽)
const int ScanlineVisibleDots = 256;
const int VisibleScanlines = 240;

const int FrameEndScanline = 261;
const int AttrbiuteOffset = 0x3c0;

class PPU
{

public:
    PPU(PictureBus &bus, VirtualScreen &screen);
    ~PPU();

    void Reset();
    void Step();
    void SetInterruptCallback(std::function<void(void)> cb);
    // OAM (object attribute memory)

    void SetMask(Byte mask);
    Byte GetStatus();
    void SetDataAddress(Byte addr);

    void SetData(Byte data);
    Byte GetData();

    void SetOAMAddress(Byte addr);
    void SetOAMData(Byte value);
    Byte GetOAMData();

    void SetScroll(Byte scroll);
    void Scroll(Byte ctl);

    void DoDMA(const Byte *page_ptr);

private:
    Byte Read(Address addr);
    Byte ReadOAM(Byte addr);
    void WriteOAM(Byte addr, Byte value);

    PictureBus &m_bus;
    VirtualScreen &m_screen;

    std::vector<Byte> m_sprite_memory;
    std::vector<Byte> m_scanline_sprites;

    enum PipelineState
    {
        PreRender,
        Render,
        PostRender,
        VerticalBlank
    } m_pipeline_state;

    int m_cycle;
    int m_scanline;
    bool event_frame;

    bool m_vblank;
    bool m_spr_zreo_hit;

    // v blank 消隐
    std::function<void(void)> m_vblank_callback;

    // 寄存器
    Address m_data_address;
    Address m_temp_address;

    Byte m_fine_x_sroll;
    bool m_first_write;

    Byte m_data_buffer;
    Byte m_sprite_data_address;

    // Flags and variables
    bool m_long_sprites;
    bool m_generate_interrupt;
    bool m_greyscale_mode;
    bool m_show_sprites;
    bool m_show_background;
    bool m_hide_edge_sprites;
    bool m_hide_edge_background;

    enum CharacterPage{
        Low,
        High
    } m_bg_page, m_spr_page;

    Address m_data_addr_increment;

    // 存颜色的vector 的vector
    // picture 缓存
    std::vector<std::vector<sf::Color>> m_picture_buffer;

};