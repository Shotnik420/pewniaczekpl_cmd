#include "player.h"

Player::Player(int PID,std::string name, float skill_football, float skill_basketball, float skill_tennis, int concussion_day_left) 
    : PID(PID),name(name), skill_football(skill_football), skill_basketball(skill_basketball), skill_tennis(skill_tennis) {  
}

Player::Player() : PID(0), name(""), skill_football(0.0), skill_basketball(0.0), skill_tennis(0.0) {
}

std::string Player::getName() const {
    return name;
}
int Player::getPID() const {
    return PID;
}

