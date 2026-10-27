#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <deque>

using namespace std;

string colors[16] = {
                        "черный", "синий", "зеленый", "сине-зеленый", 
                        "красный", "красно-синий", "коричневый", 
                        "светло-серый", "темно-серый", "ярко-синий", 
                        "ярко-зеленый", "яркий сине-зеленый", "ярко-красный", 
                        "яркий красно-синий", "желтый", "белый"
                    };

int dos_to_ansi[16] = {0, 4, 2, 6, 1, 5, 3, 7, 8, 12, 10, 14, 9, 13, 11, 15};

void gotoxy(int x, int y){ // переместить курсор в точку (x, y)
    cout << "\033[" << y << ";" << x << "H";
}

void set_colors(int fg, int bg){ // установить цвет символа и фон
    cout << "\033[38;5;" << dos_to_ansi[fg] << "m"; // символ
    cout << "\033[48;5;" << dos_to_ansi[bg] << "m"; // фон
}

void reset_colors(){ // сбросить цвета
    cout << "\033[0m";
}

void clear_screen(){ // очистить экран
    cout << "\033[2J\033[H";
}

void hide_cursor(){ // спрятать курсор
    cout << "\033[?25l";
}

void show_cursor(){ // показать курсор
    cout << "\033[?25h";
}

void sleep_ms(int ms){ // пауза (мсек)
    this_thread::sleep_for(chrono::milliseconds(ms));
}

void draw_frame(int x1, int y1, int x2, int y2){ // отрисовать рамку
    reset_colors();
    gotoxy(x1, y1);
    for(int x = x1; x <= x2; x++) cout << "-";
    gotoxy(x1, y2);
    for(int x = x1; x <= x2; x++) cout << "-";

    for(int y = y1 + 1; y <= y2 - 1; y++){
        gotoxy(x1, y); cout << "|";
        gotoxy(x2, y); cout << "|";
    }

    gotoxy(x1, y1); cout << "+";
    gotoxy(x2, y1); cout << "+";
    gotoxy(x1, y2); cout << "+";
    gotoxy(x2, y2); cout << "+";
}

void clear_inside_frame(int x1, int y1, int x2, int y2){ // очистить внутренность
    int width = x2 - x1 - 1;
    for(int y = y1 + 1; y <= y2 - 1; y++){
        gotoxy(x1 + 1, y);
        for(int i = 0; i < width; i++) cout << " ";
    }
}

int main(){
    clear_screen();
    hide_cursor();
    draw_frame(25, 8, 55, 18);
    int fg; // символ
    int bg; // фон
    int visible = 4;
    deque<int> buffer;
    for(bg = 0; bg <= 7; bg++){
        for(fg = 0; fg <= 15; fg++){
            buffer.push_front(fg);
            if(buffer.size() > visible) buffer.pop_back();
            set_colors(0, bg);
            clear_inside_frame(25, 8, 55, 18);
            for(int i = 0; i < buffer.size(); i++){
                set_colors(buffer[i], bg);
                gotoxy(27, 9 + i * 2);
                cout << "Фон: " << colors[bg] << endl;
                gotoxy(27, 9 + i * 2 + 1);
                cout << "Символ: " << colors[buffer[i]] << endl;
            }
            sleep_ms(1200);
        }
    }
    reset_colors();
    gotoxy(1, 22);
    show_cursor();
    return 0;
}