#include "common.h"
#include <unistd.h>
#include <termios.h>

using namespace std;

int get_key(){
    unsigned char c;
    read(STDIN_FILENO, &c, 1); // читаем один байт
    if(c == '\033'){ // если нажат Esc (т.е начало спецпоследовательности для стрелок)
        unsigned char c_arr[2];
        read(STDIN_FILENO, &c_arr[0], 1); 
        read(STDIN_FILENO, &c_arr[1], 1); // читаем еще два байта
        if(c_arr[0] == '[' && c_arr[1] == 'A') return 1; // вверх
        if(c_arr[0] == '[' && c_arr[1] == 'B') return 2; // вниз
    }
    if(c == 'q') return 0; // выход
    return -1; // игнориуем нажатия если ничего из того что нам нужно не было введно
}

int main(){
    struct termios old_settings; // переменная для старых дефолтных настроек консоли
    struct termios new_settings; // перменная для новых экспермиентальных настроек консоли
    tcgetattr(STDIN_FILENO, &old_settings); // записали настройки в old_settings
    new_settings = old_settings; // копирования (создание русского поля экспериментов)

    new_settings.c_lflag &= ~(ICANON | ECHO); // выключаем флаги поведения
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings); // применям новые настройки к терминалу

    int star_x = 40;
    int star_y = 13;
    clear_screen();
    hide_cursor();
    draw_frame(X1, Y1, X2, Y2);
    draw_star(star_x, star_y);

    bool running = true;
    while(running){
        int key = get_key();
        switch(key){
            case 1:
                if(star_y > Y1 + 1){
                    erase_star(star_x, star_y);
                    star_y--;
                    draw_star(star_x, star_y);
                }
                break;
            
            case 2:
                if(star_y < Y2 - 1){
                    erase_star(star_x, star_y);
                    star_y++;
                    draw_star(star_x, star_y);
                }
                break;

            case 0:
                running = false;
                break;
        }
    }

    show_cursor();
    clear_screen();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings); // возращаем настройки консоли чтобы ничего не сломалось
    
    return 0;
}