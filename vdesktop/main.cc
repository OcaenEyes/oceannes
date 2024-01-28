/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:11:51
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-28 20:32:00
 * @FilePath: \vdesktop\main.cc
 * @Description: 注释信息
 */

#include "Emulator.h"
#include "Log.h"
#include "SFML/Graphics.hpp"

#include <iostream>



int main(int argc, char **argv)
{

    // if (argc < 2)
    // {
    //     LOG(Error) << " Usage: ./OceanNes [ROM File Path]";
    //     exit(EXIT_FAILURE);
    // }
    // LOG(Info) << "Start Game!";
    // std::string romfile = argv[1];
    // emulator.Run(romfile);
    sf::RenderWindow window;
    window.create(sf::VideoMode(600, 512),
                  "OCEAN-NES", sf::Style::Titlebar | sf::Style::Close);

    sf::Texture GameBG;
    GameBG.loadFromFile("./images/bg.png");
    sf::Sprite GameBg_sprite(GameBG);
    GameBg_sprite.scale((float)window.getSize().x / (float)GameBG.getSize().x,
                        (float)window.getSize().y / (float)GameBG.getSize().y);

    while (window.isOpen())
    {
        window.draw(GameBg_sprite);
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::EventType::Closed:
                window.close();
                break;
            case sf::Event::EventType::KeyPressed:
                // 改变窗口标题当空格键按下的时候
                if (event.key.code == sf::Keyboard::Key::Space)
                    window.setTitle("Space Pressed");
                break;
            case sf::Event::EventType::KeyReleased:
                // 再次改变窗口标题当空格键松开的时候
                if (event.key.code == sf::Keyboard::Key::Space)
                    window.setTitle("Space Released");
                // 当 Esc 键松开的时候关闭窗口
                else if (event.key.code == sf::Keyboard::Key::Escape)
                    window.close();
                break;
            default:
                break;
            }
        }

        window.clear();
        window.draw(GameBg_sprite);
    
        window.display();
    }

    // Emulator emulator;
    // emulator.Run("./Super_mario_brothers.nes");
    return 0;
}