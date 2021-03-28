#!/bin/bash
rm -rf ./bin/*
cd ./build
cmake ..
#make -j$(shell grep -c ^processor /proc/cpuinfo 2>/dev/null)
make -j4
cd ..
cd ./bin
mkdir Containers Recipes input output Meta
cd ..
#cp config.json ./bin
