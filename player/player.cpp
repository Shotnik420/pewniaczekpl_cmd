#include "player.h"

int Player::object_count = 0;

Player::Player(int PID,std::string name, float skill_football, float skill_basketball, int type) 
    : PID(PID),name(name), skill_football(skill_football), 
    skill_basketball(skill_basketball), 
    type(type) { 
        object_count ++;
}

Player::Player() : PID(0), name(""), skill_football(0.0), skill_basketball(0.0), type(0) {
    object_count ++;
}

std::string Player::getName() const {
    return name;
}
int Player::getPID() const {
    return PID;
}



