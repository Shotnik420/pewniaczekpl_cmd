#ifndef KOSZYKARZ_H
#define KOSZYKARZ_H
#include "Player.h"
#include <string>

class Koszykarz : public Player {
public:
    std::string type;
    Koszykarz(int PID, std::string name, float skill_football, float skill_basketball, float skill_tennis, int concussion_day_left = 0) 
    : Player(PID, name, skill_football, skill_basketball, skill_tennis, 0,2) {}
    Koszykarz() : Player(0, "Koszykarz", 1.0f, 0.0f, 0.0f, 0, 1) {}
    ~Koszykarz() = default;

    PlayerType getPlayerType() const override { return PlayerType::Koszykarz; }

};

#endif