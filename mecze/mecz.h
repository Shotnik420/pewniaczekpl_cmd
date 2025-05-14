#ifndef MECZ_H
#define MECZ_H
#include <string>
#include <vector>
#include "../player/player.h"
class Mecz {
private:
    std::string nazwa;
    std::string opponent1;
    std::string opponent2;
    std::vector<Player> opponents1team;
    std::vector<Player> opponents2team;
    int type;
    float kurs1;
    float kurs2;
    int wynik;
    int MID;
public:
    Mecz() : nazwa(""), opponent1(""), opponent2(""), kurs1(0.1), kurs2(0.1), MID(0) {}
    Mecz(std::string nazwa, std::string opponent1, std::string opponent2, float kurs1, float kurs2, int MID);
    Mecz(int MID);

    std::string getName() const;
    int getMID() const;
    void changeMID(int value);\
    int getType() const;
    float getKurs1() const;
    float getKurs2() const;
    std::string getOpponent1() const;
    std::string getOpponent2() const;
    int getWynik() const;
private:
    void set_name();
    void set_random_opponentsteam();
    void set_random_team_names();
    void set_rand_type();
    void set_rand_kurs();
    void set_wynik();
};


#endif
