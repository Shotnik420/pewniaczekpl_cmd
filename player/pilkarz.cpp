#include "Pilkarz.h"

Pilkarz::Pilkarz(int PID, std::string name, float skill_football, float skill_basketball, float skill_tennis, int concussion_day_left) 
    : Player(PID, name, skill_football, skill_basketball, skill_tennis, concussion_day_left) {
    this->type = "Pilkarz";
    }

Pilkarz::Pilkarz() 
    : Player(0, "Pilkarz", 1.0f, 0.0f, 0.0f, 0) {}

Pilkarz::~Pilkarz() = default;


