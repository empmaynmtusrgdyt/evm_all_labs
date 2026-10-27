#!/bin/bash
set -e

g++ -std=c++17 main.cpp -o main.out \
    `pkg-config --cflags --libs sdl2 SDL2_ttf`

echo "Готово: ./main.out"