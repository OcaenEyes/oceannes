#include "Emulator.h"
#include <iostream>

Emulator::Emulator(/* args */)
{
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