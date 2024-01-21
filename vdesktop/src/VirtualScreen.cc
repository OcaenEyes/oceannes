/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:30:36
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-20 22:46:50
 * @FilePath: /vdesktop/src/VirtualScreen.cc
 * @Description: 注释信息
 */
#include "VirtualScreen.h"

/*
 *  SMFL(Simple and Fast Multimedia Library)
 *  API 文档： https://www.sfml-dev.org/documentation/2.6.1/
 */

VirtualScreen::VirtualScreen()
{
}

VirtualScreen::~VirtualScreen()
{
}

void VirtualScreen::Create(unsigned int width, unsigned int height, float pixel_size, sf::Color color)
{
    m_vertices.resize(width * height * 6);

    // 二元组
    m_screen_size = {width, height};
    m_vertices.setPrimitiveType(sf::Triangles);

    // 像素大小
    m_pixel_size = pixel_size;

    for (std::size_t i = 0; i < width; i++)
    {
        for (std::size_t k = 0; k < height; k++)
        {
            auto index = (i * m_screen_size.y + k) * 6;
            // float 型 二元组
            sf::Vector2f coord2d(i * m_pixel_size, k * m_pixel_size);

            // 上左
            m_vertices[index].position = coord2d;
            m_vertices[index].color = color;

            // 上右

            // 下右

            // 下左
        }
    }
}

void VirtualScreen::SetPixel(std::size_t x, std::size_t y, sf::Color color)
{
}

void VirtualScreen::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
}