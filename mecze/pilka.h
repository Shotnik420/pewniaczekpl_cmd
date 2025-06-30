#ifndef PILKA_H
#define PILKA_H

#include <string>
#include <vector>
#include "../player/player.h"
#include "mecz.h"
class Pilka : public Mecz {
public:
    Pilka(std::string nazwa, std::string opponent1, std::string opponent2, float kurs1, float kurs2);
    Pilka();
    ~Pilka() override;
    std::string get_next_play() override;
};

#endif