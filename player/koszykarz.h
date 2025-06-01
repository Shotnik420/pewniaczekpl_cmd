#ifndef KOSZYKARZ_H
#define KOSZYKARZ_H
#include "Player.h"
#include <string>

class Koszykarz : public Player {
private:
    int PID;
    std::string name; 
    float skill_football;
    float skill_basketball;
    float skill_tennis;
    int concussion_day_left;
public:
    Koszykarz(int PID, std::string name, float skill_football, float skill_basketball, float skill_tennis, int concussion_day_left = 0) : Player(PID, name, skill_football, skill_basketball, skill_tennis, 0) {}
    Koszykarz() : Player(0, "Koszykarz", 1.0f, 0.0f, 0.0f, 0) {}
    ~Koszykarz() = default;

    std::string Koszykarz::getName() const {
    return name;
    }
    int Koszykarz::getPID() const {
        return PID;
    }

    float Koszykarz::getBonus() const {
    }
};

#endif