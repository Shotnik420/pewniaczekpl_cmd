#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>
#include <memory>
enum class PlayerType {
    Unknown,
    Koszykarz,
    Pilkarz
};
class Player {
private:
    int PID; 
    std::string name;
    
    float skill_football;
    float skill_basketball;
    float skill_tennis;

    int concussion_day_left = 0;
    
    int type = 0;

public:
    Player(int PID, std::string name, float skill_football, float skill_basketball, float skill_tennis, int concussion_day_left = 0, int type = 0);
    Player();

    ~Player() = default;

    std::string getName() const;
    int getPID() const;

    
    float getSkillFootball() const {
        return skill_football; 
    }
    float getSkillBasketball() const {
        return skill_basketball; 
    }
    float getSkillTennis() const {
        return skill_tennis; 
    }
    int getConcussionDayLeft() const {
        return concussion_day_left; 
    }

    int getType() const {
        return type; 
    }

    virtual float getBonus() const {
        return 0.0f; 
    }
    virtual PlayerType getPlayerType() const { return PlayerType::Unknown; };
};


#endif
