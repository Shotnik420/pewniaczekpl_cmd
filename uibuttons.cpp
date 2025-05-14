#include "uibuttons.h"
#include <string.h>
#include <stdlib.h>

#define YELLOW_BORDER 2
#define DEFAULT_BORDER 1
#define UNPLAYED 3
#define COLOR_BRIGHT_YELLOW 8


void init_ui_colors() {
    start_color();

    // Niestandardowe kolory
    init_color(COLOR_YELLOW, 1000, 1000, 0);        // Żółty
    init_color(8, 1000, 500, 0);                    // Pomarańcz (ID: 8)
    init_color(9, 500, 500, 1000);                  // Jasnoniebieski (ID: 9)
    init_color(10, 0, 0, 700);                      // Ciemnoniebieski (ID: 10)
    init_color(11, 0, 1000, 0);                      // Zielony (ID: 11)
    init_color(12, 1000, 0, 0);                      // Czerwony (ID: 12)

    // Pary kolorów
    init_pair(COLOR_BRIGHT_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(YELLOW_BORDER, COLOR_YELLOW, COLOR_BLACK);
    init_pair(DEFAULT_BORDER, COLOR_WHITE, COLOR_BLACK);  // Biały
    init_pair(100, 9, COLOR_BLACK);                // Jasnoniebieski
    init_pair(101, 10, COLOR_BLACK);               // Ciemnoniebieski
    init_pair(102, 8, COLOR_BLACK);                // Pomarańczowy

    init_pair(103, 11, COLOR_BLACK);   // Zielony
    init_pair(104, 12, COLOR_BLACK);   // Czerwony
}


UIButton create_button(int x, int y, int w, int h, const char* text, int border_color) {
    UIButton btn;
    btn.xpos = x;
    btn.ypos = y;
    btn.width = w;
    btn.height = h;
    btn.text = strdup(text); // Alokacja pamięci dla tekstu
    btn.focused = false;
    btn.visible = true;
    btn.border_color_pair = border_color;
    return btn;
}


void draw_button(WINDOW* win, UIButton* btn) {
    if (!btn->visible) return;
    
    // Oblicz pozycję tekstu aby był wyśrodkowany
    int text_x = btn->xpos + (btn->width - strlen(btn->text)) / 2;
    int text_y = btn->ypos + btn->height / 2;
    

    wattron(win, COLOR_PAIR(btn->border_color_pair));

    // Rysuj ramkę przycisku
    for (int y = btn->ypos; y < btn->ypos + btn->height; y++) {
        for (int x = btn->xpos; x < btn->xpos + btn->width; x++) {
            if (y == btn->ypos || y == btn->ypos + btn->height - 1 ||
                x == btn->xpos || x == btn->xpos + btn->width - 1) {
                mvwaddch(win, y, x, btn->focused ? '#' : '*');
            }
        }
    }
    wattroff(win, COLOR_PAIR(btn->border_color_pair));
    // Rysuj tekst
    if (btn->focused) {
        wattron(win, A_REVERSE);
    }
    mvwprintw(win, text_y, text_x, "%s", btn->text);
    if (btn->focused) {
        wattroff(win, A_REVERSE);
    }
}

bool is_inside_button(UIButton* btn, int x, int y) {
    return (x >= btn->xpos && x < btn->xpos + btn->width &&
            y >= btn->ypos && y < btn->ypos + btn->height);
}

void free_button(UIButton* btn) {
    free(btn->text); // Zwolnienie zaalokowanej pamięci
}
