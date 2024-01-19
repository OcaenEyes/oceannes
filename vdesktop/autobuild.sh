#!/bin/bash

set -e

# 没有build目录则创建build目录
if [ ! -d `pwd`/build ];then
    mkdir `pwd`/build 
fi

rm -rf `pwd`/build/*

cd `pwd`/build &&
    cmake .. &&
    cmake --build .

cd ..