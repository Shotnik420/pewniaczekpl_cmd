#ifndef PILKARZ_H
#define PILKARZ_H
#include "Player.h"

class Pilkarz : public Player {
public:
    std::string type;

    Pilkarz(int PID, std::string name, float skill_football, float skill_basketball) : Player(PID, name, skill_football, skill_basketball, 1) {}
    Pilkarz() : Player(0, "Pilkarz", 1.0f, 0.0f, 1) {}
    ~Pilkarz() = default;

    // Nadpisywanie funkcji get_color by zwrócić niebieski
    int get_color() const override {
        return 106; 
    }

};

#endif