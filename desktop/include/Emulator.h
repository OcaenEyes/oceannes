/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-14 16:31:55
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-03-17 17:39:12
 * @FilePath: /oceannes/desktop/include/Emulator.h
 * @Description: 注释信息
 */
#if !defined(EMULATOR_H)
#define EMULATOR_H

#include <SFML/Graphics.hpp>
#include <chrono>

#include "CPU.h"
#include "PPU.h"
#include "MainBus.h"
#include "PictureBus.h"
#include "VirtualScreen.h"
#include "Controller.h"

using TimePoint = std::chrono::high_resolution_clock::time_point;

const int NESVideoWidth = ScanlineVisibleDots;
const int NESVideoHeight = VisibleScanlines;

class Emulator
{
private:
    /* data */
    void DMA(std::uint8_t page);
    MainBus m_bus;
    PictureBus m_pictureBus;
    CPU m_cpu;
    PPU m_ppu;
    Cartridge m_cartridge;
    std::unique_ptr<Mapper> m_mapper;

    Controller m_controller1, m_controller2;

    VirtualScreen m_emulatorScreen;
    sf::RenderWindow m_window;
    float m_screenScale;

    TimePoint m_cycleTimer;

    std::chrono::high_resolution_clock::duration m_elapsedTime;
    std::chrono::nanoseconds m_cpuCycleDuration;

public:
    Emulator(/* args */);
    ~Emulator();
    void run(std::string rom_path);
    void setVideoWidth(int width);
    void setVideoHeight(int height);
    void setVideoScale(float scale);
    void setKeys(std::vector<sf::Keyboard::Key> &p1, std::vector<sf::Keyboard::Key> &p2);
};

#endif // MACRO
