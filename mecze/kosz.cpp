#include "./kosz.h"
Kosz::Kosz(std::string nazwa, std::string opponent1, std::string opponent2, float kurs1, float kurs2)
    : Mecz(nazwa, opponent1, opponent2, kurs1, kurs2, 1) {}

Kosz::Kosz() : Mecz("", "", "", 0.0, 0.0, 1) {}

Kosz::~Kosz() {}

std::string Kosz::get_next_play(){
    if(pilka_owner == 0){
        srand(time(NULL));
        if(rand() % 2 == 0){
            pilka_owner = get_new_owner(1);
            pilka_owner_team = 1; 
        } else {
            pilka_owner = get_new_owner(2); 
            pilka_owner_team = 2;
        }
        return "Zaczyna sie mecz koszykowki! Pilke rozgrywa " + get_player_name(pilka_owner) + ".";
    } else {
        if(pilka_state == 0){ // Pasywny - prowadzenie gry
            int randValue = rand() % 3; // 0 - kozluje, 1 - podaje, 2 - rzuca

            if(randValue == 0){
                return get_player_name(pilka_owner) + " prowadzi pilke, szuka okazji...";
            } else if(randValue == 1){
                pilka_taker = get_new_owner(pilka_owner_team);
                pilka_state = 2;
                return get_player_name(pilka_owner) + " podaje pilke do kolegi z druzyny.";
            } else {
                pilka_state = 3; // Strzal
                return get_player_name(pilka_owner) + " przygotowuje sie do rzutu!";
            }

        } else if(pilka_state == 1){ // Agresywny atak
            int defender = get_new_owner(pilka_owner_team == 1 ? 2 : 1);
            Player* attacker = get_player(pilka_owner);
            Player* defender_player = get_player(defender);
            float block_chance = (defender_player->getSkillBasketball() / 
                                  (defender_player->getSkillBasketball() + attacker->getSkillBasketball())) * 0.6f 
                                  + rand() % 100 / 100.0f * 0.4f;

            pilka_state = 0;

            if(rand() % 100 < block_chance * 100){
                pilka_owner = defender;
                pilka_owner_team = (pilka_owner_team == 1 ? 2 : 1);
                return get_player_name(defender) + " blokuje rzut i przejmuje pilke!";
            }

            pilka_state = 3;
            return get_player_name(pilka_owner) + " mija obronce i rzuca!";
            
        } else if(pilka_state == 2){ // Podanie
            int interceptor = get_new_owner(pilka_owner_team == 1 ? 2 : 1);
            Player* passer = get_player(pilka_owner);
            Player* defender = get_player(interceptor);
            float intercept_chance = (defender->getSkillBasketball() / 
                                      (defender->getSkillBasketball() + passer->getSkillBasketball())) * 0.5f 
                                      + rand() % 100 / 100.0f * 0.5f;

            pilka_state = 0;

            if(rand() % 100 < intercept_chance * 100){
                add_exp(defender, 0.05, 2);
                pilka_owner = interceptor;
                pilka_owner_team = pilka_owner_team == 1 ? 2 : 1;
                pilka_taker = 0;
                return get_player_name(interceptor) + " przechwytuje podanie!";
            }

            add_exp(passer, 0.05, 2);
            pilka_owner = pilka_taker;
            pilka_taker = 0;
            return get_player_name(pilka_owner) + " otrzymuje podanie.";
            
        } else if(pilka_state == 3){ // Rzut
            Player* shooter = get_player(pilka_owner);
            int defender = get_new_owner(pilka_owner_team == 1 ? 2 : 1);
            Player* defender_player = get_player(defender);
            float block_chance = (defender_player->getSkillBasketball() / 
                                  (defender_player->getSkillBasketball() + shooter->getSkillBasketball())) * 0.4f 
                                  + rand() % 100 / 100.0f * 0.6f;

            pilka_state = 0;

            if(rand() % 100 < block_chance * 100){
                pilka_owner = defender;
                pilka_owner_team = pilka_owner_team == 1 ? 2 : 1;
                Player* owner_player = get_player(pilka_owner);
                add_exp(owner_player, 0.05, 2);
                return get_player_name(defender) + " blokuje rzut! Przejmuje pilke.";
            }

            int points = rand() % 100 < 33 ? 3 : 2; // 33% szans na rzut za 3
            set_wynik(pilka_owner_team, points);
            std::string msg = "PIEKNY RZUT! " + get_player_name(pilka_owner) + " zdobywa " + std::to_string(points) + " punkty!";
            Player* owner_player = get_player(pilka_owner);
            add_exp(owner_player, 0.10, 2);
            pilka_owner = get_new_owner(pilka_owner_team == 1 ? 2 : 1);
            pilka_owner_team = pilka_owner_team == 1 ? 2 : 1;
            pilka_state = 0;
            return msg;
        }

        return "Akcja toczy sie dalej na parkiecie...";
    }
}