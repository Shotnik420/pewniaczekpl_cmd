#include "Pilkarz.h"

Pilkarz::Pilkarz(int PID, std::string name, float skill_football, float skill_basketball) 
    : Player(PID, name, skill_football, skill_basketball) {
    this->type = "Pilkarz";
    }

Pilkarz::Pilkarz() 
    : Player(0, "Pilkarz", 1.0f, 0.0f) {}

Pilkarz::~Pilkarz() = default;


