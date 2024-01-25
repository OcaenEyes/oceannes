/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:11:51
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-25 11:00:33
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
        LOG_ERROR( " Usage: ./OceanNes [ROM File Path]");
        exit(EXIT_FAILURE);
    }
    LOG_INFO("Start Game!");
    std::string romfile = argv[1];
    emulator.Run(romfile);
    return 0;
}