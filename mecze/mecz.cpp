#include "mecz.h"
#include "team_picker.h"
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
Mecz::Mecz(std::string nazwa, std::string opponent1, std::string opponent2, float kurs1, float kurs2, int MID)
    : MID(MID), nazwa(nazwa), opponent1(opponent1), opponent2(opponent2), kurs1(kurs1), kurs2(kurs2)
{
    set_random_team_names();
    set_random_opponentsteam();
    set_name();
    set_rand_type();
    set_rand_kurs();
    set_wynik();
    
    
}
Mecz::Mecz(int MID) : MID(MID) {
    set_random_team_names();
    set_random_opponentsteam();
    set_name();
    set_rand_type();
    set_rand_kurs();
    set_wynik();


}

std::string Mecz::getName() const {
    return nazwa;
}
int Mecz::getMID() const {
    return MID;
}

int Mecz::getType() const {
    return type;
}

float Mecz::getKurs1() const {
    return kurs1;
}
float Mecz::getKurs2() const {
    return kurs2;
}

std::string Mecz::getOpponent1() const {
    return opponent1;
}
std::string Mecz::getOpponent2() const {
    return opponent2;
}

int Mecz::getWynik() const {
    return wynik;
}

void Mecz::changeMID(int value) {
    MID = value;
}

void Mecz::set_name() {
    this->nazwa = opponent1 + " vs " + opponent2;
}
void Mecz::set_random_opponentsteam() {
    std::vector<Player> players = all_players;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(players.begin(), players.end(), g);
    if (players.size() >= 10) {
        opponents1team.assign(players.begin(), players.begin() + 5);

        opponents2team.assign(players.begin() + 5, players.begin() + 10);

    } else {
        std::cerr << "Za mało graczy, aby utworzyć dwie drużyny po 5 osób.\n";
    }

}

void Mecz::set_random_team_names() {
    std::string team1 = team_names[rand() % team_names.size()];
        std::string team2;
        do {
            team2 = team_names[rand() % team_names.size()];
        } while (team2 == team1);
    this->opponent1 = team1;
    this->opponent2 = team2;
}

void Mecz::set_rand_type() {
    this->type = rand() % 2; // 0 - bb, 1 - fb
}

void Mecz::set_rand_kurs() {
    float kurs = 2.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f;
    float min_value = 1.0f;
    float reszta = kurs - 2.0f;  
    float los = static_cast<float>(rand()) / RAND_MAX * reszta;

    this->kurs1 = min_value + los;
    this->kurs2 = min_value + (reszta - los);
}
void Mecz::set_wynik() {
    this->wynik = rand() % 2;
}