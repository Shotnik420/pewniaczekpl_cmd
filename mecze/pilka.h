#ifndef PILKA_H
#define PILKA_H

#include <string>
#include <vector>
#include "../player/player.h"
#include "mecz.h"
class Pilka : public Mecz {
    private:
        // Pilka nie ma dodatkowych pól, ale może mieć własne metody lub właściwości w przyszłości
    public:
        Pilka(std::string  nazwa, std::string opponent1, std::string  opponent2, float kurs1, float kurs2, int MID) : Mecz(nazwa, opponent1, opponent2, kurs1, kurs2,MID) {

        }
        Pilka() : Mecz("", "", "", 0.0,0.0,0) {}
        ~Pilka() = default;


};

#endif