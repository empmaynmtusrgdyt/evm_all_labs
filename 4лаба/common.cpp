#include "common.h"


using namespace std;

void gotoxy(int x, int y){ // переместить курсор в точку (x, y)
    cout << "\033[" << y << ";" << x << "H" << flush;
}

void clear_screen(){ // очистить экран
    cout << "\033[H\033[J" << flush;
}

void hide_cursor(){ // спрятать курсор
    cout << "\033[?25l" << flush;
}

void show_cursor(){ // показать курсор
    cout << "\033[?25h" << flush;
}

void draw_frame(int x1, int y1, int x2, int y2){ // нарисовать рамку
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
    cout << flush;
}

void draw_star(int x, int y){ // нарисовать звездочку
    gotoxy(x, y);
    cout << "*" << flush;
}

void erase_star(int x, int y){ // стереть звездочку
    gotoxy(x, y);
    cout << " " << flush;
}