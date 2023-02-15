/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-14 16:31:55
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-14 16:37:04
 * @FilePath: /oceannes/desktop/include/Emulator.h
 * @Description: 注释信息
 */
#if !defined(EMULATOR_H)
#define EMULATOR_H

#include <SFML/Graphics.hpp>
#include "VirtualScreen.h"

class Emulator
{
private:
    /* data */
    VirtualScreen m_emulatorScreen;
    sf::RenderWindow m_window;
    float m_screenScale;

public:
    Emulator(/* args */);
    ~Emulator();
    void run();
};

#endif // MACRO
