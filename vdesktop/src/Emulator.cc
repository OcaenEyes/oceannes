/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:29:14
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-27 07:48:20
 * @FilePath: /vdesktop/src/Emulator.cc
 * @Description: 注释信息
 */
#include "Emulator.h"
#include "Log.h"

/**
 * [Player 1]
 *
 * A = J
 * B = K
 * Select = RShift
 * Start = Return
 * Up = W
 * Down = S
 * Left = A
 * Right = D
 */
std::vector<sf::Keyboard::Key> con_one = {
    sf::Keyboard::Key::J,
    sf::Keyboard::Key::K,
    sf::Keyboard::Key::RShift,
    sf::Keyboard::Key::Return,
    sf::Keyboard::Key::W,
    sf::Keyboard::Key::S,
    sf::Keyboard::Key::A,
    sf::Keyboard::Key::D,
};

/**
 * [Player 2]
 *
 * A = Numpad5
 * B = Numpad6
 * Select = Numpad8
 * Start = Numpad9
 * Up = Up
 * Down = Down
 * Left = Left
 * Right = Right
 */
std::vector<sf::Keyboard::Key> con_two = {
    sf::Keyboard::Key::Numpad5,
    sf::Keyboard::Key::Numpad6,
    sf::Keyboard::Key::Numpad8,
    sf::Keyboard::Key::Numpad9,
    sf::Keyboard::Key::Up,
    sf::Keyboard::Key::Down,
    sf::Keyboard::Key::Left,
    sf::Keyboard::Key::Right,
};

/*
 * 将 m_cpu_cycle_duration增大可方便调试
 */
Emulator::Emulator() : m_cpu(m_bus),
                       m_screen_scale(2.f),
                       m_ppu(m_picture_bus, m_emulator_screen),
                       m_cycle_timer(),
                       m_cpu_cycle_duration(std::chrono::nanoseconds(559))
{
    // 855555559
    if (!m_bus.SetReadCallback(PPUSTATUS, [&](void)
                               { return m_ppu.GetStatus(); }) ||
        !m_bus.SetReadCallback(PPUDATA, [&](void)
                               { return m_ppu.GetData(); }) ||
        !m_bus.SetReadCallback(JOY1, [&](void)
                               { return m_controller1.Read(); }) ||
        !m_bus.SetReadCallback(JOY2, [&](void)
                               { return m_controller2.Read(); }) ||
        !m_bus.SetReadCallback(OAMDATA, [&](void)
                               { return m_ppu.GetOAMData(); }))
    {
        LOG_ERROR("Critical error: Failed to set I/O callbacks");
    }

    if (!m_bus.SetWriteCallback(PPUCTL, [&](Byte b)
                                { m_ppu.Control(b); }) ||
        !m_bus.SetWriteCallback(PPUMASK, [&](Byte b)
                                { m_ppu.SetMask(b); }) ||
        !m_bus.SetWriteCallback(OAMADDR, [&](Byte b)
                                { m_ppu.SetOAMAddress(b); }) ||
        !m_bus.SetWriteCallback(PPUADDR, [&](Byte b)
                                { m_ppu.SetDataAddress(b); }) ||
        !m_bus.SetWriteCallback(PPUSCROL, [&](Byte b)
                                { m_ppu.SetScroll(b); }) ||
        !m_bus.SetWriteCallback(PPUDATA, [&](Byte b)
                                { m_ppu.SetData(b); }) ||
        !m_bus.SetWriteCallback(OAMDMA, [&](Byte b)
                                { DMA(b); }) ||
        !m_bus.SetWriteCallback(JOY1, [&](Byte b)
                                { m_controller1.Write(b); m_controller2.Write(b); }) ||
        !m_bus.SetWriteCallback(OAMDATA, [&](Byte b)
                                { m_ppu.SetOAMData(b); }))
    {
        LOG_ERROR("Critical error: Failed to set I/O callbacks");
    }

    // ppu 设置中断回调函数
    m_ppu.SetInterruptCallback([&]()
                               { m_cpu.Interrupt(CPU::NMI); });
    m_controller1.SetKeyBindings(con_one);
    m_controller2.SetKeyBindings(con_two);
}

Emulator::~Emulator()
{
}

void Emulator::Run(std::string rom_path)
{
    if (!m_cartridge.LoadFromFile(rom_path))
    {
        LOG_ERROR("Unable to laod ROM file from : %s", rom_path.c_str());
        return;
    }

    m_mapper = Mapper::CreateMapper(static_cast<Mapper::MapperType>(m_cartridge.GetMapper()), m_cartridge);

    if (!m_mapper)
    {
        LOG_ERROR("Creating Mapper failed. Probably unsupported.");
        return;
    }

    if (!m_bus.SetMapper(m_mapper.get()) ||
        !m_picture_bus.SetMapper(m_mapper.get()))
    {
        return;
    }
    m_cpu.Reset();
    m_ppu.Reset();

    m_window.create(sf::VideoMode(NESVideoWidth * m_screen_scale, NESVideoHeight * m_screen_scale), "OceanNes", sf::Style::Titlebar | sf::Style::Close);
    m_window.setVerticalSyncEnabled(true);

    // sf::CircleShape shape(NESVideoWidth);
    // // 颜色填充
    // shape.setFillColor(sf::Color::Red);

    m_emulator_screen.Create(NESVideoWidth, NESVideoHeight, m_screen_scale, sf::Color::White);
    m_cycle_timer = std::chrono::high_resolution_clock::now();
    m_elapsed_time = m_cycle_timer - m_cycle_timer;

    sf::Event event;
    bool is_focus = true, is_pause = false;

    while (m_window.isOpen())
    {
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                m_window.close();
                return;
            }
            else if (event.type == sf::Event::GainedFocus)
            {
                is_focus = true;
                m_cycle_timer = std::chrono::high_resolution_clock::now();
            }
            else if (event.type == sf::Event::LostFocus)
            {
                is_focus = false;
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F2)
            {
                is_pause = !is_pause;
                if (!is_pause)
                {
                    m_cycle_timer = std::chrono::high_resolution_clock::now();
                }
            }
            else if (is_pause && event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::F3)
            {
                for (int i = 0; i < 29781; i++) // Around one frame
                {
                    // PPU
                    m_ppu.Step();
                    m_ppu.Step();
                    m_ppu.Step();
                    // CPU
                    m_cpu.Step();
                }
            }
        }
        if (is_focus && !is_pause)
        {
            m_elapsed_time += std::chrono::high_resolution_clock::now() - m_cycle_timer;
            m_cycle_timer = std::chrono::high_resolution_clock::now();

            // 上次执行的时间大于一个CPU周期
            while (m_elapsed_time > m_cpu_cycle_duration)
            {
                // PPU
                m_ppu.Step();
                m_ppu.Step();
                m_ppu.Step();
                // CPU
                m_cpu.Step();

                m_elapsed_time -= m_cpu_cycle_duration;
            }
            // LOG_INFO("运行到这里了 %s %d", __FUNCTION__, __LINE__);
            // m_window.draw(m_emulator_screen);
            m_window.display();
            // LOG_INFO("运行到这里了 %s %d", __FUNCTION__, __LINE__);
        }
        else
        {
            // 1/60 second
            sf::sleep(sf::microseconds(1000 / 60));
            // LOG_INFO("运行到这里了 %s %d", __FUNCTION__, __LINE__);
        }
        // LOG_INFO("运行到这里了 %s %d", __FUNCTION__, __LINE__);
    }
}

void Emulator::DMA(Byte page)
{
    m_cpu.SkipDMACycles();
    auto page_ptr = m_bus.GetPagePtr(page);
    m_ppu.DoDMA(page_ptr);
}
