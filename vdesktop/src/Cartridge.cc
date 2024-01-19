/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:29:06
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-19 20:02:48
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
        LOG(ERROR) << "can not open ROM file:" << path << "\n";
        return false;
    }

    LOG(INFO) << "reading ROM from : " << path << "\n";

    // 读取.nes的 header
    std::vector<Byte> header;
    if (!rom_file.read(reinterpret_cast<char *>(&header[0]), 0x10))
    {
        /* code */
    }

    return false;
}