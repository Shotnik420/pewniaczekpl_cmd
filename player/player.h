#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>
#include <memory>
#include <ostream>

class Player {
private:
    int PID; 
    std::string name;
    
    float skill_football;
    float skill_basketball;

    
    
    int type = 0;

public:
    static int object_count;
    Player(int PID, std::string name, float skill_football, float skill_basketball, int type = 0);
    Player();

    ~Player(){
        object_count--;
    };

    std::string getName() const;
    int getPID() const;

    
    
    float getSkillFootball() const {
        return skill_football; 
    }
    float getSkillBasketball() const {
        return skill_basketball; 
    }
    int getType() const {
        return type; 
    }

    void setSkillFootball(float skill) {
        skill_football = skill; 
    }
    void setSkillBasketball(float skill) {
        skill_basketball = skill; 
    }

    virtual float getBonus() const {
        return 0.0f; 
    }

    virtual int get_color() const {
        return 0; 
    }

    //Operator overload dla pomocy zapisywania gracza do pliku;

    friend std::ostream& operator<<(std::ostream& os, const Player& player) {
        std::string player_type;
        if (player.getType() == 1){
            player_type = "Pilkarz";
        } else if(player.getType() == 2){
            player_type = "Koszykarz";
        }

        
        os << player.getPID() << " " << player.getName() << ","
        << player_type << " " << player.getSkillBasketball() << " "
        << player.getSkillFootball() << " "
        <<"\n";
        return os;
    }

};


#endif
