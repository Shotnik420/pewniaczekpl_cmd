#ifndef UIBUTTONS_H
#define UIBUTTONS_H

#include <curses.h>
#include <stdbool.h>  // Dodane dla typu bool
#include <stdlib.h>   // Dodane dla funkcji free()
#include <string.h>   // Dodane dla strdup()
void init_ui_colors();
class UIButton {
public:
    int xpos;
    int ypos;
    int width;
    int height;
    char* text;
    bool visible;
    int border_color_pair;

    UIButton(int x, int y, int w, int h, const char* text, int border_color)
        : xpos(x), ypos(y), width(w), height(h), visible(true), border_color_pair(border_color) {
        this->text = strdup(text); // Alokacja pamięci dla tekstu
    }

    UIButton() : xpos(0), ypos(0), width(0), height(0), text(nullptr), visible(true), border_color_pair(0) {
        // Domyślny konstruktor
    }

    

};
// Inicjalizacja przycisku
   UIButton create_button(int x, int y, int w, int h, const char* text, int border_color);

    // Rysowanie przycisku
    void draw_button(WINDOW* win, UIButton* btn);

    // Sprawdza czy pozycja (x,y) jest wewnątrz przycisku
    bool is_inside_button(UIButton* btn, int x, int y);

#endif