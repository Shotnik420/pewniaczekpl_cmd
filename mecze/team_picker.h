#ifndef TEAM_PICKER_H
#define TEAM_PICKER_H
#include <string>
#include <vector>
#include <memory>
#include "../player/player.h"
void initialize_players(); 
// Deklaracja zmiennych globalnych, ale bez inicjalizacji
extern std::vector<std::unique_ptr<Player>> all_players;
extern std::vector<std::string> team_names;

#endif
