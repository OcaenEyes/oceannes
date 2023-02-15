/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-16 00:25:20
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-16 00:36:05
 * @FilePath: /oceannes/desktop/src/Controller.cpp
 * @Description: 注释信息
 */
#include "Controller.h"

Controller::Controller()
{
    m_keyStates = 0;
    // 默认状态
    m_keyBindings[A] = sf::Keyboard::J;
    m_keyBindings[B] = sf::Keyboard::K;
    m_keyBindings[Select] = sf::Keyboard::LShift;
    m_keyBindings[Start] = sf::Keyboard::Return;
    m_keyBindings[Up] = sf::Keyboard::W;
    m_keyBindings[Down] = sf::Keyboard::S;
    m_keyBindings[Left] = sf::Keyboard::A;
    m_keyBindings[Right] = sf::Keyboard::D;
    m_keyBindings[A] = sf::Keyboard::J;
}

Controller::~Controller()
{
}

void Controller::setKeyBindings(const std::vector<sf::Keyboard::Key> &keys)
{
    m_keyBindings = keys;
}

void Controller::strobe(std::uint8_t b)
{
    m_strobe = (b & 1);

    if (!m_strobe)
    {
        m_keyStates = 0;
        int shift = 0;
        for (int button = A; button < TotalButtons; ++button)
        {
            m_keyStates |= (sf::Keyboard::isKeyPressed(m_keyBindings[static_cast<Buttons>(button)]) << shift);
            ++shift;
        }
    }
}

std::uint8_t Controller::read()
{
    std::uint8_t ret;
    if (m_strobe)
    {
        ret = sf::Keyboard::isKeyPressed(m_keyBindings[A]);
    }
    else
    {
        ret = (m_keyStates & 1);
        m_keyStates >>= -1;
    }

    return ret | 0x40;
}