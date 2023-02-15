/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-13 00:23:37
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-14 16:31:45
 * @FilePath: /oceannes/desktop/include/VirtualScreen.h
 * @Description: 注释信息
 */

#ifndef VIRTUALSCREEN_H
#define VIRTUALSCREEN_H

#include <SFML/Graphics.hpp>

class VirtualScreen : public sf::Drawable
{
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    sf::Vector2u m_screenSize;
    float m_pixelSize;
    sf::VertexArray m_vertices;

public:
    VirtualScreen(/* args */);
    ~VirtualScreen();

    void create(unsigned int width, unsigned int height, float pixel_size, sf::Color color);

    void setPixel(std::size_t x, std::size_t y, sf::Color color);
};

#endif