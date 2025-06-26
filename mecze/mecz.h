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
    int bet1 = 0;
    int bet2 = 0;
public:
    Mecz() : nazwa(""), opponent1(""), opponent2(""), kurs1(0.1), kurs2(0.1), MID(0) {}
    Mecz(std::string nazwa, std::string opponent1, std::string opponent2, float kurs1, float kurs2, int MID);
    Mecz(int MID);
    ~Mecz() = default;
    std::string getName() const;
    int getMID() const;
    void changeMID(int value);
    int getType() const;
    float getKurs1() const;
    float getKurs2() const;
    std::string getOpponent1() const;
    std::string getOpponent2() const;
    std::vector<Player> getOpponents1team() const { return opponents1team; }
    std::vector<Player> getOpponents2team() const { return opponents2team; }
    int getWynik() const;
    void set_bet1(int value);
    void set_bet2(int value);
    int getBet1() const { return bet1; }
    int getBet2() const { return bet2; }
    float getPayout1() const {
        if (wynik == 1) {
            return bet1 * kurs1;
        }
        return 0.0f;
    }
    float getPayout2() const {
        if (wynik == 2) {
            return bet2 * kurs2;
        }
        return 0.0f;
    }
private:
    void set_name();
    void set_random_opponentsteam();
    void set_random_team_names();
    void set_rand_type();
    void set_rand_kurs();
    void set_wynik();
};


#endif
