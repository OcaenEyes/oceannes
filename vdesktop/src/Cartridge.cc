/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:29:06
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-27 07:46:14
 * @FilePath: /vdesktop/src/Cartridge.cc
 * @Description: 注释信息
 */
#include "Cartridge.h"
#include "Log.h"

#include <fstream>

Cartridge::Cartridge() : m_name_TableMirroring(0),
                         m_mapper_number(0),
                         m_extended_RAM(false)
{
}

Cartridge::~Cartridge()
{
}

const std::vector<Byte> &Cartridge::GetRom()
{
    return m_PRG_ROM;
}

const std::vector<Byte> &Cartridge::GetVRom()
{
    return m_CHR_ROM;
}

bool Cartridge::HasExtendedRAM()
{
    return m_extended_RAM;
}

Byte Cartridge::GetMapper()
{
    return m_mapper_number;
}

Byte Cartridge::GetNameTableMirroring()
{
    return m_name_TableMirroring;
}

/*
 * .NES 文件头格式 https://wiki.nesdev.com/w/index.php/INES
 * 0-3: Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
 * 4: Size of PRG ROM in 16 KB units
 * 5: Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
 * 6: Flags 6 - Mapper, mirroring, battery, trainer
 * 7: Flags 7 - Mapper, VS/Playchoice, NES 2.0
 * 8: Flags 8 - PRG-RAM size (rarely used extension)
 * 9: Flags 9 - TV system (rarely used extension)
 * 10: Flags 10 - TV system, PRG-RAM presence (unofficial, rarely used extension)
 * 11-15: Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15
 */
bool Cartridge::LoadFromFile(std::string path) 
{
    std::ifstream rom_file(path, std::ios_base::binary | std::ios_base::in);
    if (!rom_file)
    {
        LOG_ERROR("can not open ROM file: %s", path.c_str());
        return false;
    }

    LOG_INFO("reading ROM from : %s ", path.c_str());

    // 读取.nes的 header
    std::vector<Byte> header;
    header.resize(0x10);

    if (!rom_file.read(reinterpret_cast<char *>(&header[0]), 0x10))
    {
        LOG_ERROR("reading nes header failed!");
        return false;
    }

    if (std::string{&header[0], &header[4]} != "NES\x1A")
    {
        LOG_ERROR("not a valid nes image, magic number: %s %hhu %hhu  %hhu %d valid magic number: N E S 1a", std::hex, header[0], header[1], header[2], int(header[3]));
        return false;
    }
    LOG_INFO("reading header, it dictates: \n -*----------------------*-\n");

    Byte banks = header[4];
    LOG_INFO("16KB PRG-ROM Banks: %hhu", +banks);
    if (!banks)
    {
        LOG_ERROR("ROM has no PRG-ROM banks, loading ROM failed.");
        return false;
    }

    // video banks
    Byte vbanks = header[5];
    LOG_INFO("8KB CHR-ROM Banks: %hhu", +vbanks);

    // name table mirroring
    m_name_TableMirroring = header[6] & 0xB;
    LOG_INFO("Name Table Mirroring: %d", +m_name_TableMirroring);

    // mapper number
    m_mapper_number = ((header[6] >> 4) & 0xf) | (header[7] & 0xf0);
    LOG_INFO("Mapper number #: %d", +m_mapper_number);

    m_extended_RAM = header[6] & 0x2;
    // LOG_INFO("Extended (CPU) RAM  %s %s", std::boolalpha, m_extended_RAM);

    // 暂不支持  使用Trainer格式的 .nes文件
    if (header[6] & 0x4)
    {
        LOG_ERROR("PAL ROM not supported.");
        return false;
    }

    if ((header[0xA] & 0x3) == 0x2 || (header[0xA] & 0x1))
    {
        LOG_ERROR("PAL ROM not supported.");
        return false;
    }
    else
    {
        LOG_INFO(" ROM is NTSC compatible.");
    }

    // PRG-ROM 16KB banks
    // 将0x4000(16KB)  * banks 内容写入到 m_PRG_ROM中
    m_PRG_ROM.resize(0x4000 * banks);
    if (!rom_file.read(reinterpret_cast<char *>(&m_PRG_ROM[0]), 0x4000 * banks))
    {
        LOG_ERROR("reading PRG-ROM from image file failed!");
        return false;
    }

    // CHR-ROM 8KB banks
    if (vbanks)
    {
        m_CHR_ROM.resize(0x2000 * vbanks);
        if (!rom_file.read(reinterpret_cast<char *>(&m_CHR_ROM[0]), 0x2000 * vbanks))
        {
            LOG_ERROR("reading CHR-ROM from image file failed!");
            return false;
        }
    }
    else
    {
        LOG_INFO("Cartridge with CHR-RAM.");
    }

    LOG_INFO("-*---------------------------*-");
    return true;
}