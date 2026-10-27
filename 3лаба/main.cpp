#include <iostream>
#include <cmath>
#include <SDL.h>
#include <SDL_ttf.h>

using namespace std;

const int N = 2000; // кол-во точек; почему именно 2000? потому что 10 периодов функции на интервале, по 200 точек на период — достаточно для гладкого отображения без лишней нагрузки на проц

const int plot_left = 80; // левый край графика
const int plot_right = 760; // правый край графика
const int plot_top = 40; // верхний край графика
const int plot_bottom = 540; // нижний край графика

const double x_min = 3 * M_PI / 2; // начало интерала
const double x_max = 8 * M_PI; // конец интервала
const double y_min = -1.2; // тк [-1...1]
const double y_max = 1.2; // аналогично

const char* x_labels[14] = {
    "3π/2", "2π", "5π/2", "3π", "7π/2", "4π", "9π/2",
    "5π", "11π/2", "6π", "13π/2", "7π", "15π/2", "8π"
}; // корды точек на оси x

const char* y_labels[5] = { "-1", "-0.5", "0", 
"0.5", "1" }; // корды точке по оси y

struct Point{
    double x;
    double y;
};

/*
1 - инициализация
2 - окно
3 - рендер
4 - инициализация шрифта
5 - создание щрифта
6 - второе окно
7 - второй рендер
*/

// g++ main.cpp -o main.out `pkg-config --cflags --libs sdl2 SDL2_ttf`


// ф-ция для рисования подписей на оси x
void draw_text_x(SDL_Renderer* rend, TTF_Font* font, const char* text, int cent_x, int top_y){
    SDL_Color color = {255, 255, 255, 255}; // белый

    SDL_Surface* surf = TTF_RenderUTF8_Solid(font, text, color);
    if(surf == nullptr) return;

    int w = surf->w; // ширина surf
    int h = surf->h; // высота surf

    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surf);
    if(texture == nullptr){
        SDL_FreeSurface(surf);
        return;
    }
    SDL_FreeSurface(surf);

    int x = cent_x - w / 2; // считаем левый край текста
    SDL_Rect rect = {x, top_y, w, h}; // создаем прямоуголник
    SDL_RenderCopy(rend, texture, nullptr, &rect); // типа ресуем
    SDL_DestroyTexture(texture);
}


// ф-ция для рисования подписей на оси y
void draw_text_y(SDL_Renderer* rend, TTF_Font* font, const char* text, int right_x, int cent_y){
    SDL_Color color = {255, 255, 255, 255}; // белый

    SDL_Surface* surf = TTF_RenderUTF8_Solid(font, text, color);
    if(surf == nullptr) return;

    int w = surf->w; // ширина surf
    int h = surf->h; // высота surf

    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, surf);
    if(texture == nullptr){
        SDL_FreeSurface(surf);
        return;
    }
    SDL_FreeSurface(surf);

    int x = right_x - w; // считаем правый край текста
    int y = cent_y - h / 2;
    SDL_Rect rect = {x, y, w, h}; // создаем прямоуголник
    SDL_RenderCopy(rend, texture, nullptr, &rect); // типа ресуем
    SDL_DestroyTexture(texture);
}


// ф-ции для преобразования математических значений x и y в пиксели
int screen_x(double x){
    int pixel_x = plot_left + (x - x_min) / (x_max - x_min) * (plot_right - plot_left);
    return pixel_x;
}

int screen_y(double y){
    int pixel_y = plot_bottom - (y - y_min) / (y_max - y_min) * (plot_bottom - plot_top);
    return pixel_y;
}


int main(int argc, char* argv[]){
    // инициализация SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        cout << "SDL_Init error: " << SDL_GetError() << endl;
        return 1;
    }


    // инициализация шрифта
    if(TTF_Init() != 0){
        cout << "TTF_Init error: " << TTF_GetError() << endl;
        SDL_Quit();
        return 4;
    }


    // создание окна
    SDL_Window* window = SDL_CreateWindow(
        "3лаба: график",
        200, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );
    if(window == nullptr){
        cout << "SDL_CreateWindow error: " << SDL_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        return 2;
    }

    // создание второго окна
    SDL_Window* window_2 = SDL_CreateWindow(
        "3лаба: максимум",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        400, 150,
        SDL_WINDOW_SHOWN
    );
    if(window_2 == nullptr){
        cout << "SDL_CreateWindow error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 6;
    }
    int c1, c2;
    SDL_GetWindowPosition(window, &c1, &c2);
    SDL_SetWindowPosition(window_2, c1 + 820, c2 + 50);


    // создание рендера (дингус для рисования)
    SDL_Renderer* rend = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if(rend == nullptr){
        cout << "SDL_CreateRenderer error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_DestroyWindow(window_2);
        TTF_Quit();
        SDL_Quit();
        return 3;
    }

    // создание второго рендера
    SDL_Renderer* rend_2 = SDL_CreateRenderer(
        window_2, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if(rend_2 == nullptr){
        cout << "SDL_CreateRenderer error: " << SDL_GetError() << endl;
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_DestroyWindow(window_2);
        TTF_Quit();
        SDL_Quit();
        return 7;
    }


    // создание шрифта
    char* base = SDL_GetBasePath();
    string font_path = string(base) + "LiberationSans-Regular.ttf";
    SDL_free(base);
    TTF_Font* font = TTF_OpenFont(font_path.c_str(), 14);
    if(font == nullptr){
        cout << "TTF_OpenFont error: " << TTF_GetError() << endl;
        SDL_DestroyRenderer(rend_2);
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window_2);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 5;
    }

    // гененрация точек
    struct Point point_arr[N];
    for(int i = 0; i < N; i++){
        double x = x_min + i * (x_max - x_min) / (N - 1);
        double y = sin(x) * sin(x) * sin(x) + cos(x) * cos(x);
        point_arr[i].x = x;
        point_arr[i].y = y;
    }


    // ищем минимум и максимум
    double max_x = point_arr[0].x;
    double max_y = point_arr[0].y;
    for(int i = 0; i < N; i++){
        if(point_arr[i].y > max_y){
            max_y = point_arr[i].y;
            max_x = point_arr[i].x;
        }
    }

    //cout << max_y << " " << max_x << endl; - 1 25.132

    int mx = screen_x(max_x);
    int my = screen_y(max_y);

    string s = "Максимум: " + to_string(max_y) + ", при x = " + to_string(max_x);


    bool running = true;
    SDL_Event event;

    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT) running = false;
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE) running = false;
            }
        }
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255); // черная кисть
        SDL_RenderClear(rend);
        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); // белая кисть

        //SDL_Rect frame = {plot_left, plot_top, plot_right - plot_left, plot_bottom - plot_top}; // описываем прямоугольник
        //SDL_RenderDrawRect(rend, &frame); // и рисуем его

        double d = (0.0 - y_min) / (y_max - y_min); // доля
        int axis_x_y = plot_bottom - d * (plot_bottom - plot_top); // y-корда оси x
        SDL_RenderDrawLine(rend, plot_left, axis_x_y, plot_right, axis_x_y);

        int axis_y_x = plot_left + 2; // x-корда оси y
        SDL_RenderDrawLine(rend, axis_y_x, plot_top, axis_y_x, plot_bottom);

        int i_for_x = 0;
        for(double x = x_min; x <= x_max + 0.001; x += M_PI / 2){
            int px = screen_x(x);
            SDL_RenderDrawLine(rend, px, axis_x_y - 5, px, axis_x_y + 5);
            draw_text_x(rend, font, x_labels[i_for_x], px, axis_x_y + 10);
            i_for_x++;
        }

        int i_for_y = 0;
        for(double y = -1.0; y <= 1.0 + 0.001; y += 0.5){
            int py = screen_y(y);
            SDL_RenderDrawLine(rend, axis_y_x - 5, py, axis_y_x + 5, py);
            draw_text_y(rend, font, y_labels[i_for_y], axis_y_x - 10, py);
            i_for_y++;
        }

        SDL_SetRenderDrawColor(rend, 52, 201, 36, 255); // вердепомовый (оттенок зеленого)
        for(int i = 0; i < N - 1; i++){ // рисуем график
            int x1 = screen_x(point_arr[i].x);
            int y1 = screen_y(point_arr[i].y);
            int x2 = screen_x(point_arr[i + 1].x);
            int y2 = screen_y(point_arr[i + 1].y);

            SDL_RenderDrawLine(rend, x1, y1, x2, y2);
        }

        // рисуем крестик там где максимум (хотя вообще их много просто значения совпадают)
        SDL_SetRenderDrawColor(rend, 229, 43, 80, 255); // амарантовый (оттенок красного)
        SDL_RenderDrawLine(rend, mx - 5, my, mx + 5, my);
        SDL_RenderDrawLine(rend, mx, my - 5, mx, my + 5);


        SDL_RenderPresent(rend); // показываем готовый кадр для первого окна


        SDL_SetRenderDrawColor(rend_2, 0, 0, 0, 255); // черная кисть
        SDL_RenderClear(rend_2);
        SDL_SetRenderDrawColor(rend_2, 255, 255, 255, 255); // белая кисть

        draw_text_x(rend_2, font, s.c_str(), 150, 30);
        SDL_RenderPresent(rend_2);
    }


    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(rend_2);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window_2);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}