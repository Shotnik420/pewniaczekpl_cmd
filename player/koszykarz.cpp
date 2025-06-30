#include "Koszykarz.h"

Koszykarz::Koszykarz(int PID, std::string name, float skill_football, float skill_basketball) 
    : Player(PID, name, skill_football, skill_basketball, 2) {
        this->type = "Koszykarz";
    }

Koszykarz::Koszykarz() 
    : Player(0, "Koszykarz", 1.0f, 0.0f, 0) {}

Koszykarz::~Koszykarz() = default;




