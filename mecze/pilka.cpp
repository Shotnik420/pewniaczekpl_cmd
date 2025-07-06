#include "./pilka.h"

Pilka::Pilka(std::string nazwa, std::string opponent1, std::string opponent2, float kurs1, float kurs2)
    : Mecz(nazwa, opponent1, opponent2, kurs1, kurs2, 0) {}
Pilka::Pilka() : Mecz("", "", "", 0.0, 0.0, 0) {}
Pilka::~Pilka() {}
// Logika gry w piłkę nożną
std::string Pilka::get_next_play(){
    if(pilka_owner == 0){
        srand(time(NULL));
        if(rand()%2==0){
            pilka_owner = get_new_owner(1);
            pilka_owner_team = 1; 
        }else{
            pilka_owner = get_new_owner(2); 
            pilka_owner_team = 2;
        }

        return "Zaczyna sie mecz! Gre rozpoczyna "+ get_player_name(pilka_owner) + ".";
    }
    else{
        if(pilka_state == 0){ // Pasywny
            int randValue = rand() % 3; // 0 - gra na czas, 1 - podaje, 2 - atakuje
            
            if(randValue == 0){ // Gra na czas
                return get_player_name(pilka_owner) + " oglada sie.. Szuka jakiejs szansy...";
            }
            else if (randValue == 1){ // Podaje
                if(pilka_owner_team == 1) pilka_taker = get_new_owner(1); else pilka_taker = get_new_owner(1);
                pilka_state = 2; 
                return get_player_name(pilka_owner) + " podaje pilke do kolegi z druzyny.";
            }
            else if (randValue == 2){ // Atakuje, zmienia swoj stan
                pilka_state = 1;
                return get_player_name(pilka_owner) + " atakuje! Szarzuje na bramke przeciwnika!";
            }
        }
        else if (pilka_state == 1){ // Agresywny
            
            int interceptor = get_new_owner(pilka_owner_team == 1 ? 2 : 1);
            Player* pilka_owner_player = get_player(pilka_owner);
            Player* interceptor_player = get_player(interceptor);
            float intercept_chance = (interceptor_player->getSkillFootball() / (interceptor_player->getSkillFootball() + pilka_owner_player->getSkillFootball()) * 0.7f + rand() % 100 / 100.0f * 0.3f);
            pilka_state = 0; 
            if(rand() % 100 < intercept_chance * 100){ // Przeciwnik przejmuje piłkę
                pilka_owner = interceptor;
                pilka_taker = 0;
                pilka_owner_team = pilka_owner_team == 1 ? 2 : 1; // Zmienia drużynę
                return get_player_name(interceptor) + " pokazuje klasowa obrone i zdobywa pilke dla druzyny!";
            }
            int randValue = rand() % 2; // 0 - podaje, 1 - strzela
            if(randValue == 0){ // Podaje
                if(pilka_owner_team == 1) pilka_taker = get_new_owner(1); else pilka_taker = get_new_owner(2);
                pilka_state = 2; 
                return get_player_name(pilka_owner) + " podaje pilke do kolegi z druzyny.";
            }else { // Strzela
                pilka_state = 3; 
                return get_player_name(pilka_owner) + " strzela na bramke!";
            }

            
           
        }
        else if (pilka_state == 2){ // Podanie
            int interceptor = get_new_owner(pilka_owner_team == 1 ? 2 : 1);
            Player* pilka_owner_player = get_player(pilka_owner);
            Player* interceptor_player = get_player(interceptor);
            float intercept_chance = (interceptor_player->getSkillFootball() / (interceptor_player->getSkillFootball() + pilka_owner_player->getSkillFootball()) * 0.7f + rand() % 100 / 100.0f * 0.3f);
            pilka_state = 0; 
             // Dodaje exp do pilkarza
            if(rand() % 100 < intercept_chance * 100){ // Przeciwnik przejmuje piłkę
                add_exp(interceptor_player, 0.10, 1);
                pilka_taker = interceptor;
                pilka_owner_team = pilka_owner_team == 1 ? 2 : 1; // Zmienia drużynę
                pilka_owner = pilka_taker;
                return get_player_name(interceptor) + " przecina podanie i przejmuje pilke!";
            }

            add_exp(pilka_owner_player, 0.10, 1);

            pilka_state = 0; // Resetuje stan
            pilka_owner = pilka_taker;
            pilka_taker = 0; // Resetuje pilke_taker
            return get_player_name(pilka_owner) + " odbiera pilke.";
        }
        else if (pilka_state == 3){ // Strzał
            int interceptor = get_new_owner(pilka_owner_team == 1 ? 2 : 1);
            Player* pilka_owner_player = get_player(pilka_owner);
            Player* interceptor_player = get_player(interceptor);
            float intercept_chance = (interceptor_player->getSkillFootball() / (interceptor_player->getSkillFootball() + pilka_owner_player->getSkillFootball()) * 0.7f + rand() % 100 / 100.0f * 0.3f);
            pilka_state = 0; 
            if(rand() % 100 < intercept_chance * 100){ // Przeciwnik przejmuje piłkę
                pilka_taker = interceptor;
                pilka_owner_team = pilka_owner_team == 1 ? 2 : 1; // Zmienia drużynę
                pilka_owner = pilka_taker;
                Player* interceptor_player = get_player(pilka_owner);
                add_exp(interceptor_player , 0.10, 1);
                return get_player_name(interceptor) + " broni bramke przed strzalem!";
            }
            pilka_owner_team = pilka_owner_team == 1 ? 2 : 1; // Zmienia drużynę
            int scorer = pilka_owner; 
            int pilka_owner = get_new_owner(pilka_owner_team);
            pilka_taker = 0;
            if(get_pilka_team() == 1){
                set_wynik(1, 1);
            }else{
                set_wynik(2, 1);
            }
            Player* score_player = get_player(scorer);
            add_exp(score_player , 0.10, 1);

            return "GOOOOOL" + get_player_name(scorer) + " strzela perfekcyjnie!";
            
        }else if(pilka_state == 4){ // Gola zdobył przeciwnik
            pilka_owner = pilka_taker;
            pilka_taker = 0; // Resetuje pilke_taker
            pilka_state = 0; // Resetuje stan
            return get_player_name(pilka_owner) + " wznowił grę po stracie gola.";
        }
        
        return "Kolejna akcja w meczu.";
    }
}