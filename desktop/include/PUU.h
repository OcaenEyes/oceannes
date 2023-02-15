/*
 * @Author: OCEAN.GZY
 * @Date: 2023-02-15 23:55:34
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2023-02-16 00:16:12
 * @FilePath: /oceannes/desktop/include/PUU.h
 * @Description: 注释信息
 */
#if !defined(PPU_H)
#define PPU_H

#include <functional>
#include <array>
#include "PictureBus.h"
#include "MainBus.h"
#include "VirtualScreen.h"
#include "PaletteColors.h"

const int ScanlineCycleLength = 341;
const int ScanlineEndCycle = 340;
const int VisibleScanlines = 240;
const int ScanlineVisibleDots = 256;
const int FrameEndScanline = 261;

const int AttributeOffset = 0x3c0;

class PUU
{
private:
    /* data */
    std::uint8_t readOAM(std::uint8_t addr);
    void writeOAM(std::uint8_t addr, std::uint8_t value);
    std::uint8_t read(std::uint16_t addr);
    PictureBus &m_bus;
    VirtualScreen &m_screen;

    std::function<void(void)> m_vblankCallback;

    std::vector<std::uint8_t> m_spriteMemory;

    std::vector<std::uint8_t> m_scanlineSprites;

    enum State
    {
        PreRender,
        Render,
        PostRender,
        VerticalBlank
    } m_pipelineState;

    int m_cycle;
    int m_scanline;
    bool m_evenFrame;

    bool m_vblank;
    bool m_sprZeroHit;
    bool m_spriteOverflow;

    // Registers
    std::uint16_t m_dataAddress;
    std::uint16_t m_tempAddress;
    std::uint8_t m_fineXScroll;

    bool m_firstWrite;
    std::uint8_t m_dataBuffer;

    std::uint8_t m_spriteDataAddress;

    bool m_longSprites;
    bool m_generateInterrupt;

    bool m_greyscaleMode;
    bool m_showSprites;
    bool m_showBackground;
    bool m_hideEdgeSprites;
    bool m_hideEdgeBackground;

    enum CharacterPage
    {
        Low,
        High
    } m_bgPage,
        m_sprPage;

    std::uint16_t m_dataAddrIncrement;

    std::vector<std::vector<sf::Color>> m_pictureBuffer;

public:
    PUU(PictureBus &bus, VirtualScreen &screen);

    void step();
    void reset();

    void setInterruptCallback(std::function<void(void)> cb);

    void doDMA(const std::uint8_t *page_ptr);

    // Callbacks mapped to CPU address space
    // Addresses written to by the program
    void control(std::uint8_t ctrl);
    void setMask(std::uint8_t mask);
    void setOAMAddress(std::uint8_t addr);
    void setDataAddress(std::uint8_t addr);

    void setScroll(std::uint8_t scroll);
    void setData(std::uint8_t data);

    // read
    std::uint8_t getStatus();
    std::uint8_t getData();
    std::uint8_t getOAMData();

    void setOAMData(std::uint8_t value);

    ~PUU();
};

#endif // PPU_H
