#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>

class Player {
private:
    int PID; 
    std::string name;
    
    float skill_football;
    float skill_basketball;
    float skill_tennis;

    int concussion_day_left = 0;
    
    

public:
    Player(int PID, std::string name, float skill_football, float skill_basketball, float skill_tennis, int concussion_day_left = 0);
    Player();

    ~Player() = default;

    std::string getName() const;
    int getPID() const;

    virtual float getBonus() const {
        return 0.0f; 
    }
};


#endif
