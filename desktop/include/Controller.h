/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-16 00:17:50
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-16 00:21:41
 * @FilePath: /oceannes/desktop/include/Controller.h
 * @Description: 注释信息
 */
#if !defined(CONTROLLER_H)
#define CONTROLLER_H

#include <SFML/Window.hpp>
#include <cstdint>
#include <vector>
class Controller
{
private:
    /* data */
    bool m_strobe;
    unsigned int m_keyStates;

    std::vector<sf::Keyboard::Key> m_keyBindings;

public:
    Controller(/* args */);
    ~Controller();

    enum Buttons
    {
        A,
        B,
        Select,
        Start,
        Up,
        Down,
        Left,
        Right,
        TotalButtons,
    };

    void strobe(std::uint8_t b);
    std::uint8_t read();

    void setKeyBindings(const std::vector<sf::Keyboard::Key> &keys);
};

#endif // CONTROLLER_H
