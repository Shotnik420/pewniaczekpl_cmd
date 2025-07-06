#include "uibuttons.h"
#include <string.h>
#include <stdlib.h>

#define YELLOW_BORDER 2
#define DEFAULT_BORDER 1
#define UNPLAYED 3
#define COLOR_BRIGHT_YELLOW 8
#define ORANGE 102
#define DARKBLUE 101
#define LIGHTBLUE 100
#define GREEN 103
#define RED 104
#define LIGHTGREEN 105
#define LIGHTBLUE2 106
#define LIGHTORANGE 107

void init_ui_colors() { // Tworzenie nowych par kolorów
    start_color();

    // Niestandardowe kolory
    init_color(COLOR_YELLOW, 1000, 1000, 0);        // Żółty
    init_color(8, 1000, 500, 0);                    // Pomarańcz (ID: 8)
    init_color(9, 500, 500, 1000);                  // Jasnoniebieski (ID: 9)
    init_color(10, 400, 200, 700);                      // Ciemnoniebieski (ID: 10)
    init_color(11, 0, 1000, 0);                      // Zielony (ID: 11)
    init_color(12, 1000, 0, 0);                      // Czerwony (ID: 12)
    init_color(13, 700, 1000, 700);                 // Jasnozielony (ID: 13)
    init_color(14, 700, 900, 1000);  // Jasnoniebieski (ID: 14)
    init_color(15, 1000, 800, 500);  // Jasnopomarańczowy (ID: 15)
    // Pary kolorów
    init_pair(COLOR_BRIGHT_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(YELLOW_BORDER, COLOR_YELLOW, COLOR_BLACK);
    init_pair(DEFAULT_BORDER, COLOR_WHITE, COLOR_BLACK);  // Biały
    init_pair(100, 9, COLOR_BLACK);                // Jasnoniebieski
    init_pair(101, 10, COLOR_BLACK);               // Ciemnoniebieski
    init_pair(102, 8, COLOR_BLACK);                // Pomarańczowy

    init_pair(103, 11, COLOR_BLACK);   // Zielony
    init_pair(104, 12, COLOR_BLACK);   // Czerwony
    
    init_pair(105, 13, COLOR_BLACK); // Para kolorów z jasnozielonym na czarnym tle
    init_pair(106, 14, COLOR_BLACK); // Jasnoniebieski na czarnym tle
    init_pair(107, 15, COLOR_BLACK); // Jasnopomarańczowy na czarnym tle
    init_pair(108, COLOR_BLACK,COLOR_WHITE); // Jasnopomarańczowy na czarnym tle

}


//// Funkcja tworząca nowy przycisk
UIButton create_button(int x, int y, int w, int h, const char* text, int border_color) {
    UIButton btn(x,y,w,h,text,border_color);
    return btn;
}


void UIButton::draw_button(WINDOW* win) {
    if (!visible) return;
    
    // Oblicz pozycję tekstu aby był wyśrodkowany
    int text_x = xpos + (width - strlen(text)) / 2;
    int text_y = ypos + height / 2;
    

    wattron(win, COLOR_PAIR(border_color_pair));

    // Rysuj ramkę przycisku
    for (int y = ypos; y < ypos + height; y++) {
        for (int x = xpos; x < xpos + width; x++) {
            if (y == ypos || y == ypos + height - 1 ||
                x == xpos || x == xpos + width - 1) {
                mvwaddch(win, y, x, '*');
            }
        }
    }
    
    // Rysuj tekst
    mvwprintw(win, text_y, text_x, "%s", text);
    wattroff(win, COLOR_PAIR(border_color_pair));
}

//Czy kursor myszy znajduje się wewnątrz przycisku
bool UIButton::is_inside_button(int x, int y) {
    return (x >= xpos && x < xpos + width &&
            y >= ypos && y < ypos + height);
}

