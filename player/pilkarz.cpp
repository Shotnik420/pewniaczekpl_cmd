#include "Pilkarz.h"

Pilkarz::Pilkarz(int PID, std::string name, float skill_football, float skill_basketball, float skill_tennis, int concussion_day_left) 
    : Player(PID, name, skill_football, skill_basketball, skill_tennis, concussion_day_left) {}

Pilkarz::Pilkarz() 
    : Player(0, "Pilkarz", 1.0f, 0.0f, 0.0f, 0) {}

Pilkarz::~Pilkarz() = default;
std::string Pilkarz::getName() const {
    return name;
}
int Pilkarz::getPID() const {
    return PID;
}

float Pilkarz::getBonus() const {
    return skill_football * 1.3f; // Example bonus calculation based on football skill
}