/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 01:06:32
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-15 01:33:54
 * @FilePath: /oceannes/desktop/include/MainBus.h
 * @Description: 注释信息
 */
#if !defined(MAINBUS_MEMEORY_H)
#define MAINBUS_MEMEORY_H

#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include "Cartridge.h"
#include "Mapper.h"

enum IORegisters
{
    PPUCTRL = 0x2000,
    PPUMASK,
    PPUSTATUS,
    OAMADDR,
    OAMDATA,
    PPUSCROL,
    PPUADDA,
    PPUDATA,
    OAMDMA = 0x4014,
    JOY1 = 0x4016,
    JOY2 = 0x4017,
};

struct IORegistersHasher
{
    std::size_t operator()(IORegisters const &reg) const noexcept
    {
        return std::hash<std::uint32_t>{}(reg);
    };
};

class MainBus
{
private:
    /* data */
    std::vector<std::uint8_t> m_RAM;
    std::vector<std::uint8_t> m_extRAM;
    Mapper *m_mapper;

public:
    MainBus(/* args */);
    ~MainBus();

    std::uint8_t read(std::uint16_t addr);
    void write(std::uint16_t addr, std::uint8_t value);

    bool setMapper(Mapper *mapper);

    bool setWriteCallback(IORegisters reg, std::function<void(std::uint8_t)> callback);
    bool setReadCallback(IORegisters reg, std::function<std::uint8_t(void)> callback);

    const std::uint8_t *getPagePtr(std::uint8_t page);
};

MainBus::MainBus(/* args */)
{
}

MainBus::~MainBus()
{
}

#endif // MAINBUS_MEMEORY_H
