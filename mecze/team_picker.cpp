#include "team_picker.h"
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include "../player/player.h"
#include "../player/koszykarz.h"
#include "../player/pilkarz.h"
std::vector<std::unique_ptr<Player>> all_players;
void initialize_players(std::string user_name) {

    
    //Jeżeli istnieje plik sportsmen.txt, to wczytaj z niego graczy
    //Inaczej wczytaj domyślnych graczy
    std::ifstream file("./dane/"+user_name+"/sportsmen.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            int id;
            std::string name, type;
            double skill1, skill2;
            if (!(iss >> id >> std::ws)) continue;
            if (!std::getline(iss, name, ',')) continue;
            if (!(iss >> type >> skill1 >> skill2)) continue;
            // Remove leading/trailing spaces from name
            name.erase(0, name.find_first_not_of(" \t"));
            name.erase(name.find_last_not_of(" \t") + 1);
            if (type == "Koszykarz") {
                all_players.push_back(std::make_unique<Koszykarz>(id, name, skill1, skill2));
            } else if (type == "Pilkarz") {
                all_players.push_back(std::make_unique<Pilkarz>(id, name, skill1, skill2));
            }
        }
        file.close();
        return;
    }


    all_players.push_back(std::make_unique<Koszykarz>(1, "Janusz Wieprzowina", 0.97, 0.33));
    all_players.push_back(std::make_unique<Koszykarz>(2, "Tata swinka", 0.6, 0.9));
    all_players.push_back(std::make_unique<Koszykarz>(3, "Szakal Mongolski", 0.5, 0.8));
    all_players.push_back(std::make_unique<Koszykarz>(4, "Tymone Roodi", 0.5, 0.6));
    all_players.push_back(std::make_unique<Koszykarz>(5, "Freaky Jopa", 0.5, 0.7));
    all_players.push_back(std::make_unique<Koszykarz>(6, "Gruby Ztefdwa", 0.4, 0.7));
    all_players.push_back(std::make_unique<Pilkarz>(7, "Ulrich Jungingen", 0.8, 0.4));
    all_players.push_back(std::make_unique<Pilkarz>(8, "Polski Vip", 0.3, 0.9));
    all_players.push_back(std::make_unique<Pilkarz>(9, "Chlebak Kacper", 0.6, 0.8));
    all_players.push_back(std::make_unique<Pilkarz>(10, "Tkaczyski Patricio", 0.7, 0.6));
    all_players.push_back(std::make_unique<Pilkarz>(11, "Koszmarny Karolek", 0.8, 0.5));
    all_players.push_back(std::make_unique<Pilkarz>(12, "Kamil Caputnik", 0.9, 0.4));
    all_players.push_back(std::make_unique<Pilkarz>(13, "DJKhaled", 0.5, 0.3));
    all_players.push_back(std::make_unique<Koszykarz>(14, "Polski Mlot", 0.1, 2.1));
    all_players.push_back(std::make_unique<Koszykarz>(15, "Zenek Martyniuk", 0.5, 0.5));
}


//Zapisz wszystkich graczy do pliku sportsmen.txt
//Używa przeciążenia operatora << w klasie Player
void save_players(const std::string& user_name) {
    std::ofstream file("./dane/" + user_name + "/sportsmen.txt");
    if (!file.is_open()) return;
    for (const auto& player : all_players) {
        file << *player << std::endl;
    }
    file.close();
}

std::vector<std::string> team_names = {"Team AEI", "Sejaki Rozrabiaki", "MS za plotem", "Gorole czestochowskie", "Gornik Chorzuf", "Draze Corsarze"};
