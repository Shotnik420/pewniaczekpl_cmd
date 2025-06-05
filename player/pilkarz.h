#ifndef PILKARZ_H
#define PILKARZ_H
#include "Player.h"

class Pilkarz : public Player {
public:
    std::string type;

    Pilkarz(int PID, std::string name, float skill_football, float skill_basketball, float skill_tennis, int concussion_day_left = 0) : Player(PID, name, skill_football, skill_basketball, skill_tennis, 0,1) {}
    Pilkarz() : Player(0, "Pilkarz", 1.0f, 0.0f, 0.0f, 0,1) {}
    ~Pilkarz() = default;

    PlayerType getPlayerType() const override { return PlayerType::Pilkarz; }



};

#endif