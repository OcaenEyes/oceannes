/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:29:06
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-22 18:03:59
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
    header.resize(0x10);

    if (!rom_file.read(reinterpret_cast<char *>(&header[0]), 0x10))
    {
        LOG(ERROR) << "reading nes header failed!\n";
        return false;
    }

    if (std::string{&header[0], &header[4]} != "NES\x1A")
    {
        LOG(ERROR) << "not a valid nes image, magic number: " << std::hex << header[0] << " " << header[1] << " " << header[2] << " " << int(header[3]) << "\n"
                   << "valid magic number: N E S 1a\n";
        return false;
    }
    LOG(INFO) << "reading header, it dictates: \n -*----------------------*-\n";

    Byte banks = header[4];
    LOG(INFO) << "16KB PRG-ROM Banks: " << +banks << "\n";
    if (!banks)
    {
        LOG(ERROR) << "ROM has no PRG-ROM banks, loading ROM failed. " << +banks << "\n";
        return false;
    }

    // video banks
    Byte vbanks = header[5];
    LOG(INFO) << "8KB CHR-ROM Banks: " << +vbanks << "\n";

    // name table mirroring
    m_name_TableMirroring = header[6] & 0xB;
    LOG(INFO) << "Name Table Mirroring: " << +m_name_TableMirroring << "\n";

    // mapper number
    m_mapper_number = ((header[6] >> 4) & 0xf) | (header[7] & 0xf0);
    LOG(INFO) << "Mapper number #: " << +m_mapper_number << "\n";

    m_extended_RAM = header[6] & 0x2;
    LOG(INFO) << "Extended (CPU) RAM " << std::boolalpha << m_extended_RAM << "\n";

    // 暂不支持  使用Trainer格式的 .nes文件
    if (header[6] & 0x4)
    {
        LOG(ERROR) << "PAL ROM not supported.\n";
        return false;
    }

    if ((header[0xA] & 0x3) == 0x2 || (header[0xA] & 0x1))
    {
        LOG(ERROR) << "PAL ROM not supported.\n";
        return false;
    }
    else
    {
        LOG(INFO) << " ROM is NTSC compatible.\n";
    }

    // PRG-ROM 16KB banks
    // 将0x4000(16KB)  * banks 内容写入到 m_PRG_ROM中
    m_PRG_ROM.resize(0x4000 * banks);
    if (!rom_file.read(reinterpret_cast<char *>(&m_PRG_ROM[0]), 0x4000 * banks))
    {
        LOG(ERROR) << "reading PRG-ROM from image file failed!\n";
        return false;
    }

    // CHR-ROM 8KB banks
    if (vbanks)
    {
        m_CHR_ROM.resize(0x2000 * vbanks);
        if (!rom_file.read(reinterpret_cast<char *>(&m_CHR_ROM[0]), 0x2000 * vbanks))
        {
            LOG(ERROR) << "reading CHR-ROM from image file failed!\n";
            return false;
        }
    }
    else
    {
        LOG(INFO) << "Cartridge with CHR-RAM." << std::endl;
    }

    LOG(INFO) << "-*---------------------------*-\n";
    return true;
}