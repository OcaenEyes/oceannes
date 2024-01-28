/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:11:51
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-28 12:09:46
 * @FilePath: /vdesktop/main.cc
 * @Description: 注释信息
 */

#include "Emulator.h"
#include "Log.h"

#include <iostream>

int main(int argc, char **argv)
{
    Emulator emulator;
    if (argc < 2)
    {
        LOG(Error) << " Usage: ./OceanNes [ROM File Path]";
        exit(EXIT_FAILURE);
    }
    LOG(Info) << "Start Game!";
    std::string romfile = argv[1];
    emulator.Run(romfile);
    return 0;
}