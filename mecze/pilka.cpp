#include "./pilka.h"

Pilka::Pilka(std::string nazwa, std::string opponent1, std::string opponent2, float kurs1, float kurs2, int MID)
    : Mecz(nazwa, opponent1, opponent2, kurs1, kurs2, MID) {}
Pilka::Pilka() : Mecz("", "", "", 0.0, 0.0, 0) {}

