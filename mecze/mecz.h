#ifndef MECZ_H
#define MECZ_H
#include <string>
#include <vector>
#include "../player/player.h"
#include "../Bet/bet.h"
class Mecz {
private:
    std::string nazwa;
    std::string opponent1name;
    std::string opponent2name;
    std::vector<Player*> opponents1team;
    std::vector<Player*> opponents2team;

    std::vector<float> team1_skill_change;
    std::vector<float> team2_skills_change;

    int type;
    float kurs1;
    float kurs2;
    int wynik1 = 0;
    int wynik2 = 0;

    Bet bet1;
    Bet betRemis;
    Bet bet2;



    
public:
    Mecz() : nazwa(""), opponent1name(""), opponent2name(""), kurs1(0.1), kurs2(0.1), type(0) {}
    Mecz(std::string nazwa, std::string opponent1, std::string opponent2, float kurs1, float kurs2, int type);



    int pilka_owner = 0;
    int pilka_taker = 0;
    int pilka_state = 0; // 0 - Pasywny, 1 - Aktywny, 2 - Podanie, 3 - Strzał
    int pilka_owner_team = 0;


    std::string get_player_name(int id);
    std::vector<Player*> getOpponents1team() const { return opponents1team; }
    std::vector<Player*> getOpponents2team() const { return opponents2team; }
    int getWynik() const;

    std::string getName() const {
        return nazwa;
    }

    int getType() const {
        return type;
    }

    float getKurs1() const {
        return kurs1;
    }
    float getKurs2() const {
        return kurs2;
    }

    std::string getOpponent1() const {
        return opponent1name;
    }
    std::string getOpponent2() const {
        return opponent2name;
    }

    void set_bet1(int value);
    void set_betRemis(int value);
    void set_bet2(int value);
    int getBet1() const { return bet1.get_bet(); }
    int getBet2() const { return bet2.get_bet(); }
    int getBetRemis() const { return betRemis.get_bet(); }
    float getPayout() const {
        if (wynik1 > wynik2) {
            return bet1.get_bet() * kurs1;
        }else if(wynik1<wynik2){
            return bet2.get_bet()* kurs2;
        }else if(wynik1==wynik2){
            return betRemis.get_bet() * 2.0;
        }
        return 0.0f;
    }

    int getWynik1() const { return wynik1; }
    int getWynik2() const { return wynik2; }
    
    Player* get_player(int id);
    virtual int get_needed_skill() const { return 1; };
    
    virtual std::string get_next_play() {
        return "Brak rodzaju meczu";
    }

    virtual ~Mecz() = default;
    int get_pilka_team() {
        return pilka_owner_team;
    }

    std::vector<float> get_team1_skill_change() const {
        return team1_skill_change;
    }
    std::vector<float> get_team2_skill_change() const {
        return team2_skills_change;
    }
    std::pair<int, int> find_player_team_and_index(int id);
    void make_name();
    void set_random_opponentsteam();
    void set_random_team_names();
    void set_rand_kurs();
    void set_wynik(int who, int amount);
    void change_pilka_owner(int id);
    int get_new_owner(int team);
    void add_exp(Player* &player, float exp, int which);
};


#endif
