#ifndef KOSZYKARZ_H
#define KOSZYKARZ_H
#include "Player.h"
#include <string>

class Koszykarz : public Player {
public:
    std::string type;
    Koszykarz(int PID, std::string name, float skill_football, float skill_basketball) 
    : Player(PID, name, skill_football, skill_basketball, 2) {}
    Koszykarz() : Player(0, "Koszykarz", 1.0f, 0.0f, 2) {}
    ~Koszykarz() = default;

    // Nadpisywanie funkcji get_color by zwrócić pomarańczowy
    int get_color() const override {
        return 102; 
    }
};

#endif