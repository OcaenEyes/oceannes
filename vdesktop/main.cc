/*
 * @Author: OCEAN.GZY
 * @Date: 2024-01-19 16:11:51
 * @LastEditors: OCEAN.GZY
 * @LastEditTime: 2024-01-22 19:55:03
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
        LOG(ERROR) << " Usage: ./OceanNes [ROM File Path]\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "start Game before!\n";

    LOG(INFO) << "Start Game!\n";
    std::cout << "start Game after!\n";
    std::string romfile = argv[1];
    emulator.Run(romfile);
    return 0;
}