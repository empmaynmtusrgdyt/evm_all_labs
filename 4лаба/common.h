#pragma once

#include <iostream>

void gotoxy(int x, int y);
void clear_screen();
void hide_cursor();
void show_cursor();
void draw_frame(int x1, int y1, int x2, int y2);
void draw_star(int x, int y);
void erase_star(int x, int y);

constexpr int X1 = 25;
constexpr int Y1 = 8;
constexpr int X2 = 55;
constexpr int Y2 = 18;