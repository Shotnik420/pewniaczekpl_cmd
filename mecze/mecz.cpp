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
    
    auto all_players_copy = std::vector<std::unique_ptr<Player>>{};
    all_players_copy.reserve(all_players.size()); // pre-allocate for efficiency

    for (const auto& player : all_players) {
        all_players_copy.push_back(std::make_unique<Player>(*player));
    }

    

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(all_players_copy.begin(), all_players_copy.end(), g);
    if (all_players.size() >= 10) {
        for (int i = 0; i < 5; ++i) {
            opponents1team.push_back(*all_players_copy[i]);
        }
        for (int i = 5; i < 10; ++i) {
            opponents2team.push_back(*all_players_copy[i]);
        }


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
