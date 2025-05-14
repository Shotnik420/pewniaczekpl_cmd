#include "team_picker.h"
#include <vector>
#include "../player/player.h"

// Definicja zmiennych globalnych
std::vector<Player> all_players = {
    Player(1, "Lebron Hames", 0.97, 0.33, 0.43),
    Player(2, "Anna Nowak", 0.6, 0.9, 0.5),
    Player(3, "Piotr Wiśniewski", 0.7, 0.8, 0.9),
    Player(4, "Katarzyna Wójcik", 0.5, 0.6, 0.8),
    Player(5, "Tomasz Lewandowski", 0.9, 0.5, 0.7),
    Player(6, "Zofia Kowalczyk", 0.4, 0.7, 0.6),
    Player(7, "Jakub Zieliński", 0.8, 0.4, 0.5),
    Player(8, "Marta Kaczmarek", 0.3, 0.9, 0.4),
    Player(9, "Krzysztof Mazur", 0.6, 0.8, 0.3),
    Player(10, "Agnieszka Jabłońska", 0.7, 0.6, 0.2),
    Player(11, "Michał Pawlak", 0.8, 0.5, 0.1),
    Player(12, "Natalia Król", 0.9, 0.4, 0.3),
    Player(13, "Marcin Szymański", 0.5, 0.3, 0.2),
};

std::vector<std::string> team_names = {"Team Alpha", "Team Bravo", "Team Delta", "Team Omega", "Team Nova", "Team Blaze"};
