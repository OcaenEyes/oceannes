/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:25:27
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-28 12:03:12
 * @FilePath: /vdesktop/include/VirtualScreen.h
 * @Description: 注释信息
 */
#pragma once

#include "SFML/Graphics.hpp"

/**
 * 在一个普通成员函数后面加上const修饰，就是常成员函数
    返回值 函数名（形参表）const {函数体}
    注： （函数声明和实现分离时）const是函数声明的一部分，在函数的实现部分也需要加上const

    作用
        特点：
        ①可以使用数据成员，不能进行修改，对函数的功能有更明确的限定；（本类数据成员有“mutable”关键字修饰时可以通过常成员函数修改。）；
        ②常成员函数不能用来更新类的任何成员变量，也不能调用类中未用const修饰的成员函数，只能调用常成员函数。即常成员函数不能更改类中的成员状态，这与const语义相符。
        ③常函数能修改传入自身的形参以及内部定义的局部变量；
        ④常函数的this指针是const Class*型；
        ⑤常对象只能调用常函数，不能调用普通函数；

    意义:  在实现只读作用的函数中，可以防止程序员不小心修改了成员变量的值。
*/
class VirtualScreen : public sf::Drawable
{
public:
    void Create(unsigned int width, unsigned int height, float pixel_size, sf::Color color);
    void SetPixel(std::size_t x, std::size_t y, sf::Color color);

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    // unsigned vector
    sf::Vector2u m_screenSize;
    float m_pixelSize;
    // 顶点 class
    sf::VertexArray m_vertices;
};
