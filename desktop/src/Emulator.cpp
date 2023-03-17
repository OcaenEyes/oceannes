/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-14 16:32:01
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-03-17 17:48:24
 * @FilePath: /oceannes/desktop/src/Emulator.cpp
 * @Description: 注释信息
 */
#include "Emulator.h"
#include "CPUOpcodes.h"
#include "LogUtil.h"

#include <thread>
#include <chrono>
#include <iostream>

Emulator::Emulator(/* args */) : m_cpu(m_bus),
                                 m_ppu(m_pictureBus, m_emulatorScreen),
                                 m_screenScale(3.f),
                                 m_cycleTimer(),
                                 m_cpuCycleDuration(std::chrono::nanoseconds(559))
{
    if (!m_bus.setReadCallback(PPUSTATUS, [&](void)
                               { return m_ppu.getStatus(); }) ||
        !m_bus.setReadCallback(PPUDATA, [&](void)
                               { return m_ppu.getData(); }) ||
        !m_bus.setReadCallback(JOY1, [&](void)
                               { return m_controller1.read(); }) ||
        !m_bus.setReadCallback(JOY2, [&](void)
                               { return m_controller2.read(); }) ||
        !m_bus.setReadCallback(OAMDATA, [&](void)
                               { return m_ppu.getOAMData(); }))
    {
        /* code */
        LOG(Error) << "Critical error: Failed to set I/O callbacks" << std::endl;
    }

    if (!m_bus.setWriteCallback(PPUCTRL, [&](std::uint8_t b)
                                { m_ppu.control(b); }) ||
        !m_bus.setWriteCallback(PPUMASK, [&](std::uint8_t b)
                                { m_ppu.setMask(b); }) ||
        !m_bus.setWriteCallback(OAMADDR, [&](std::uint8_t b)
                                { m_ppu.setOAMAddress(b); }) ||
        !m_bus.setWriteCallback(PPUADDR, [&](std::uint8_t b)
                                { m_ppu.setDataAddress(b); }) ||
        !m_bus.setWriteCallback(PPUSCROL, [&](std::uint8_t b)
                                { m_ppu.setScroll(b); }) ||
        !m_bus.setWriteCallback(PPUDATA, [&](std::uint8_t b)
                                { m_ppu.setData(b); }) ||
        !m_bus.setWriteCallback(OAMDMA, [&](std::uint8_t b)
                                { DMA(b); }) ||
        !m_bus.setWriteCallback(JOY1, [&](std::uint8_t b)
                                { m_controller1.strobe(b);m_controller2.strobe(b); }) ||
        !m_bus.setWriteCallback(OAMDATA, [&](std::uint8_t b)
                                { m_ppu.setOAMData(b); }))
    {
        /* code */
        LOG(Error) << "Critical error: Failed to set I/O callbacks" << std::endl;
    }

    m_ppu.setInterruptCallback([&]()
                               { m_ppu.interrupt(InterruptType::NMI) });
}

Emulator::~Emulator()
{
}

void Emulator::run()
{
    m_window.create(sf::VideoMode(600, 400), "OceanNes", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    m_window.setVerticalSyncEnabled(true);
    m_emulatorScreen.create(600, 400, m_screenScale, sf::Color::White);
    std::cout << "模拟器" << std::endl;

    sf::Event event;
    bool focus = true, pause = false;
    while (m_window.isOpen())
    {
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                m_window.close();
                return;
            }
            else if (event.type == sf::Event::LostFocus)
                focus = false;
        }

        if (focus && !pause)
        {
            m_window.draw(m_emulatorScreen);
            m_window.display();
        }
    }
}