#include "mecz.h"
#include "team_picker.h"
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
Mecz::Mecz(std::string nazwa, std::string opponent1, std::string opponent2, float kurs1, float kurs2, int type)
    :  nazwa(nazwa), opponent1name(opponent1), opponent2name(opponent2), kurs1(kurs1), kurs2(kurs2), type(type)
{
    set_random_team_names();
    set_random_opponentsteam();
    make_name();
    set_rand_kurs();
    team1_skill_change.resize(5, 0.0f);
    team2_skills_change.resize(5, 0.0f);
    
}



std::string Mecz::get_player_name(int id)
{
    for (const auto& player : all_players) {
        if (player->getPID() == id) {
            return player->getName();
        }
    }
    return std::string("NULL");
}
Player* Mecz::get_player(int id) {
    for (const auto& playerPtr : all_players) {
        if (playerPtr->getPID() == id) {
            return playerPtr.get();
        }
    }
    throw std::runtime_error("Player with ID " + std::to_string(id) + " not found.");
}




void Mecz::make_name() {
    this->nazwa = opponent1name + " vs " + opponent2name;
}
void Mecz::set_random_opponentsteam() {
    
    auto all_players_copy = std::vector<Player*>{};
    all_players_copy.reserve(all_players.size()); 

    for (const auto& player : all_players) {
        all_players_copy.push_back(player.get());
    }

    

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(all_players_copy.begin(), all_players_copy.end(), g);
    if (all_players.size() >= 10) {
        for (int i = 0; i < 5; ++i) {
            opponents1team.push_back(all_players_copy[i]);
        }
        for (int i = 5; i < 10; ++i) {
            opponents2team.push_back(all_players_copy[i]);
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
    this->opponent1name = team1;
    this->opponent2name = team2;
}


void Mecz::set_rand_kurs() {
    float t1Skill = 0.0;
    float t2Skill = 0.0;

    for(Player* player : opponents1team){
        float needed_skill = 0.0;
        switch(get_needed_skill()){
            case 1:
                needed_skill = player->getSkillFootball();
                break;
            case 2:
                needed_skill = player->getSkillBasketball();
                break;
        }
        t1Skill += needed_skill;
    }
    for(Player* player : opponents2team){
        float needed_skill = 0.0;
        switch(get_needed_skill()){
            case 1:
                needed_skill = player->getSkillFootball();
                break;
            case 2:
                needed_skill = player->getSkillBasketball();
                break;
        }
        t2Skill += needed_skill;
    }

    float chance = t1Skill/(t2Skill+t1Skill);


    this->kurs1 = 1/chance;
    this->kurs2 = 1/(1-chance);
}
void Mecz::set_wynik(int who, int amount) {
    if(who == 1){
        wynik1 += amount;
    } else if (who == 2) {
        wynik2 += amount;
    } else {
        std::cerr << "Nieprawidłowy numer drużyny: " << who << ". Wynik nie został zmieniony.\n";
    }
}

void Mecz::set_bet1(int value) {
    bet1.set_bet(value);
}
void Mecz::set_bet2(int value) {
    bet2.set_bet(value);
}
void Mecz::set_betRemis(int value){
    betRemis.set_bet(value);
}


void Mecz::change_pilka_owner(int id) {
    pilka_owner = id;
    Player* theGuy = get_player(pilka_owner);
    
}


int Mecz::get_new_owner(int team){
    int new_owner = 0;
    if(team==1){
        new_owner = opponents1team[rand() % opponents1team.size()]->getPID();
        
    }else{
        new_owner = opponents2team[rand() % opponents2team.size()]->getPID();
    }
    if (new_owner == pilka_owner) {
        new_owner = get_new_owner(team);
    }
    return new_owner;
}




void Mecz::add_exp(Player* &player, float exp, int which) {

    //Ten fragment kodu jest tylko po to by po meczu pokazać zmiane umiejętności gracza
    //Znajdź pozycje gracza w tabeli i w dodaj exp w tej samej pozycji w odpowiedniej drużynie
    std::pair<int, int> para = find_player_team_and_index(player->getPID());
    if (para.first == 1){
                team1_skill_change[para.second] += exp;
    } else if (para.first == 2) {
                team2_skills_change[para.second] += exp;
    }

    
    switch(which) {
        case 1: // Football
            player->setSkillFootball(player->getSkillFootball() + exp);
            break;
        case 2: // Basketball
            player->setSkillBasketball(player->getSkillBasketball() + exp);
            break;
        default:
            std::cerr << "Nieprawidłowy typ umiejętności.\n";
    }

}

std::pair<int, int> Mecz::find_player_team_and_index(int id) {
    for (size_t i = 0; i < opponents1team.size(); ++i) {
        if (opponents1team[i]->getPID() == id) {
            return {1, static_cast<int>(i)};
        }
    }
    for (size_t i = 0; i < opponents2team.size(); ++i) {
        if (opponents2team[i]->getPID() == id) {
            return {2, static_cast<int>(i)};
        }
    }
    return {-1, -1}; // Not found
}