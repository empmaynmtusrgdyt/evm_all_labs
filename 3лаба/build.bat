@echo off

if "%SDL2_DIR%"=="" set SDL2_DIR=C:\SDL2

g++ -std=c++17 main.cpp -o main.exe ^
    -I"%SDL2_DIR%\include" ^
    -L"%SDL2_DIR%\lib" ^
    -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

echo Готово: main.exe