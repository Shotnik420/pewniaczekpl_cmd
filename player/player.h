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

    
public:
    Player(int PID, std::string name, float skill_football, float skill_basketball, float skill_tennis);
    Player();

    std::string getName() const;
    int getPID() const;

};


#endif
