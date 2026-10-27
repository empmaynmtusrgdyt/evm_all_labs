/*
Суть второй программы: мы создаем два потока. 
Один (Производитель) постоянно слушает клавиатуру и кидает коды в очередь. 
Второй (Потребитель, то есть наш main) спит, пока очередь пуста, 
а как только там что-то появляется — забирает код и двигает звездочку.
*/


#include "common.h"
#include <unistd.h>
#include <termios.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;


queue<int> buffer; // буфер
mutex mx; // замок на двери этого буфера
condition_variable cv; // звонок для пробуждения этого буфера
bool running = true;

void producer(){ // ф-ция производитель
    while(running){
        unsigned char c;
        read(STDIN_FILENO, &c, 1); // считали байт

        if(c == '\033'){ // Esc
            unsigned char c_arr[2];
            read(STDIN_FILENO, &c_arr[0], 1);
            read(STDIN_FILENO, &c_arr[1], 1);

            if(c_arr[0] == '[' && c_arr[1] == 'A'){
                lock_guard<mutex> lock(mx); // захватили мьютекс
                buffer.push(1);
                cv.notify_one();
            }

            if(c_arr[0] == '[' && c_arr[1] == 'B'){
                lock_guard<mutex> lock(mx); // захватили мьютекс
                buffer.push(2);
                cv.notify_one();
            }
        }

        if(c == 'q'){
            running = false;
            cv.notify_all(); // будим всех кто спит (для корректно завершения программы)
        }
    }
}


int get_key_from_buffer(){ // ф-ция потребитель
    unique_lock<mutex> lock(mx); // захватили замок с возможностью его отпустить
    while(buffer.empty() && running){ // пока буфер пуст и прогрмма работает - спим
        cv.wait(lock); // замок открывается для производителя, чтобы он мог положить туда данные
    }
    if(!running) return 0;
    int key = buffer.front();
    buffer.pop();
    return key;
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

    thread prod_thread(producer); // запускаем фоновый поток-производитель
    while(running){
        int key = get_key_from_buffer();
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
                cv.notify_all(); // будим все потоки
                break;
        }
    }
    
    prod_thread.join(); // дожидаемся заверешения потока-производителя

    show_cursor();
    clear_screen();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings); // возращаем настройки консоли чтобы ничего не сломалось
    
    return 0;
}