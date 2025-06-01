#include "team_picker.h"
#include <vector>
#include <memory>
#include "../player/player.h"
std::vector<std::unique_ptr<Player>> all_players;
void initialize_players() {
    all_players.push_back(std::make_unique<Player>(1, "Lebron Hames", 0.97, 0.33, 0.43));
    all_players.push_back(std::make_unique<Player>(2, "Anna Nowak", 0.6, 0.9, 0.5));
    all_players.push_back(std::make_unique<Player>(3, "Piotr Wiśniewski", 0.7, 0.8, 0.9));
    all_players.push_back(std::make_unique<Player>(4, "Katarzyna Wójcik", 0.5, 0.6, 0.8));
    all_players.push_back(std::make_unique<Player>(5, "Tomasz Lewandowski", 0.9, 0.5, 0.7));
    all_players.push_back(std::make_unique<Player>(6, "Zofia Kowalczyk", 0.4, 0.7, 0.6));
    all_players.push_back(std::make_unique<Player>(7, "Jakub Zieliński", 0.8, 0.4, 0.5));
    all_players.push_back(std::make_unique<Player>(8, "Marta Kaczmarek", 0.3, 0.9, 0.4));
    all_players.push_back(std::make_unique<Player>(9, "Krzysztof Mazur", 0.6, 0.8, 0.3));
    all_players.push_back(std::make_unique<Player>(10, "Agnieszka Jabłońska", 0.7, 0.6, 0.2));
    all_players.push_back(std::make_unique<Player>(11, "Michał Pawlak", 0.8, 0.5, 0.1));
    all_players.push_back(std::make_unique<Player>(12, "Natalia Król", 0.9, 0.4, 0.3));
    all_players.push_back(std::make_unique<Player>(13, "Marcin Szymański", 0.5, 0.3, 0.2));
}

std::vector<std::string> team_names = {"Team Alpha", "Team Bravo", "Team Delta", "Team Omega", "Team Nova", "Team Blaze"};
