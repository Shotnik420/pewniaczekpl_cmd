#ifndef UIBUTTONS_H
#define UIBUTTONS_H

#include <curses.h>
#include <stdbool.h>  // Dodane dla typu bool
#include <stdlib.h>   // Dodane dla funkcji free()
#include <string.h>   // Dodane dla strdup()
typedef struct {
    int xpos;
    int ypos;
    int width;
    int height;
    char* text;
    bool focused;
    bool visible;
    int border_color_pair;
} UIButton;


void init_ui_colors();
// Inicjalizacja przycisku
UIButton create_button(int x, int y, int w, int h, const char* text, int border_color);

// Rysowanie przycisku
void draw_button(WINDOW* win, UIButton* btn);

// Sprawdza czy pozycja (x,y) jest wewnątrz przycisku
bool is_inside_button(UIButton* btn, int x, int y);

void free_button(UIButton* btn);

#endif