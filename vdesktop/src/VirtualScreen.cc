/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:30:36
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-25 14:48:21
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

    for (std::size_t x = 0; x < width; ++x)
    {
        for (std::size_t y = 0; y < height; ++y)
        {
            // 神坑， 之前这边没注意乘6
            auto index = (x * m_screen_size.y + y) * 6;
            // float 型 二元组
            sf::Vector2f coord2d(x * m_pixel_size, y * m_pixel_size);
            // 上左
            m_vertices[index].position = coord2d;
            m_vertices[index].color = color;

            // 上右
            m_vertices[index + 1].position = coord2d + sf::Vector2f{m_pixel_size, 0};
            m_vertices[index + 1].color = color;

            // 下右
            m_vertices[index + 2].position = coord2d + sf::Vector2f{m_pixel_size, m_pixel_size};
            m_vertices[index + 2].color = color;

            // bottom-right
            m_vertices[index + 3].position = coord2d + sf::Vector2f{m_pixel_size, m_pixel_size};
            m_vertices[index + 3].color = color;

            // bottom-left
            m_vertices[index + 4].position = coord2d + sf::Vector2f{0, m_pixel_size};
            m_vertices[index + 4].color = color;

            // top-left
            m_vertices[index + 5].position = coord2d;
            m_vertices[index + 5].color = color;
        }
    }
}

void VirtualScreen::SetPixel(std::size_t x, std::size_t y, sf::Color color)
{
    auto index = (x * m_screen_size.y + y) * 6;
    if (index >= m_vertices.getVertexCount())
    {
        return;
    }

    // Triangle-1
    // top-left
    m_vertices[index].color = color;
    // top right
    m_vertices[index + 1].color = color;
    // bottom right
    m_vertices[index + 2].color = color;

    // Triangle-2
    // bottom right
    m_vertices[index + 3].color = color;
    // bottom left
    m_vertices[index + 4].color = color;
    // top left
    m_vertices[index + 5].color = color;
}

void VirtualScreen::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Draw primitives defined by a vertex buffer.
    target.draw(m_vertices, states);
}