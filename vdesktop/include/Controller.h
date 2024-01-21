/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-20 11:54:07
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-21 12:30:03
 * @FilePath: \vdesktop\include\Controller.h
 * @Description: 注释信息
 */
#pragma once

#include "Chip.h"
#include "SFML/Window.hpp"

#include <vector>

class Controller
{
private:
    bool m_strobe;
    unsigned int m_button_states;

    // sfml库的 key类型绑定各个按键
    std::vector<sf::Keyboard::Key> m_key_bindings;

public:
    enum Buttion
    {
        A,
        B,
        Select,
        Start,
        Up,
        Down,
        Left,
        Right,
        TotalButtons
    };

    Controller(/* args */);
    ~Controller();

    void Write(Byte b);
    // 获取各个按键状态
    Byte Read();

    // 映射到红白机的 各个button的关系
    void SetKeyBindings(const std::vector<sf::Keyboard::Key> &keys);
};
