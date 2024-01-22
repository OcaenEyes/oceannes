/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:26:27
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-22 14:17:27
 * @FilePath: /vdesktop/include/Emulator.h
 * @Description: 注释信息
 */
#pragma once

#include "CPU.h"
#include "Cartridge.h"
#include "MainBus.h"
#include "Mapper.h"
#include "VirtualScreen.h"
#include "PPU.h"
#include "PictureBus.h"
#include "Controller.h"

#include "SFML/Graphics.hpp"

#include <chrono>

const int NESVideoWidth = ScanlineVisibleDots;
const int NESVideoHeight = VisibleScanlines;

class Emulator
{

public:
    Emulator(/* args */);
    ~Emulator();

    void Run(std::string rom_path);

private:
    void DMA(Byte page);

    CPU m_cpu;
    PPU m_ppu;
    PictureBus m_picture_bus;
    VirtualScreen m_emulator_screen;

    Cartridge m_cartridge;
    MainBus m_bus;

    std::unique_ptr<Mapper> m_mapper;
    sf::RenderWindow m_window;

    // 两个游戏手柄
    Controller m_controller1, m_controller2;
    // 控制屏幕缩放
    float m_screen_scale;
    // 计时
    std::chrono::high_resolution_clock::time_point m_cycle_timer;
    std::chrono::high_resolution_clock::duration m_elapsed_time;
    std::chrono::nanoseconds m_cpu_cycle_duration;
};
