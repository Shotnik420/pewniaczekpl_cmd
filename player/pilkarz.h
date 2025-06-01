#ifndef PILKARZ_H
#define PILKARZ_H
#include "Player.h"
#include <string>

class Pilkarz : public Player {
private:
    int PID;
    std::string name; 
    float skill_football;
    float skill_basketball;
    float skill_tennis;
    int concussion_day_left;
public:
    Pilkarz(int PID, std::string name, float skill_football, float skill_basketball, float skill_tennis, int concussion_day_left = 0) : Player(PID, name, skill_football, skill_basketball, skill_tennis, 0) {}
    Pilkarz() : Player(0, "Pilkarz", 1.0f, 0.0f, 0.0f, 0) {}
    ~Pilkarz() = default;

    std::string Pilkarz::getName() const {
    return name;
    }
    int Pilkarz::getPID() const {
        return PID;
    }

    float Pilkarz::getBonus() const {
    }
};

#endif