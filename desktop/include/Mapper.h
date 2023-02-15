/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 00:42:23
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-15 01:01:52
 * @FilePath: /oceannes/desktop/include/Mapper.h
 * @Description: 注释信息
 */
#if !defined(MAPPER_H)
#define MAPPER_H

#include "CPUOpcodes.h"
#include "Cartridge.h"
#include <memory>
#include <functional>

enum NameTableMirroring
{
    Horizontal = 0,
    Vertical = 1,
    FourScreen = 8,
    OneScreenLower,
    OneScreenHighter
};

class Mapper
{
public:
    enum Type
    {
        NORM = 0,
        SxROM = 1,
        UxROM = 2,
        CNROM = 3,
        MMC3 = 4,
        AxROM = 7,
        ColorDreams = 11,
        GxROM = 66,
    };

    Mapper(Cartridge &cart, Type t) : m_cartridge(cart), m_type(t){};
    ~Mapper();

    virtual void writePRG(std::uint16_t addr, std::uint8_t value) = 0;
    virtual std::uint8_t readPRG(std::uint16_t addr) = 0; // =0表示纯虚函数 纯虚函数只有函数的名字而不具备函数的功能，不能被调用。它只是通知编译系统: “在这里声明一个虚函数，留待派生类中定义”。在派生类中对此函数提供定义后，它才能具备函数的功能，可被调用。

    virtual void writeCHR(std::uint16_t addr, std::uint8_t value) = 0;
    virtual std::uint8_t readCHR(std::uint16_t addr) = 0;

    virtual NameTableMirroring getNameTableMirroring();

    bool inline hasExtenedRAM()
    {
        return m_cartridge.hasExtendedRAM();
    }

    virtual void scanlineIRQ();

    static std::unique_ptr<Mapper> createMapper(Type mapper_t, Cartridge &cart, std::function<void()> interrupt_cb, std::function<void(void)> mirroring_cb);

protected:
    /* data */
    Cartridge &m_cartridge;
    Type m_type;
};

#endif // MAPPER_H
