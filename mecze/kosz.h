#ifndef KOSZ_H
#define KOSZ_H
#include <string>
#include <vector>
#include "../player/player.h"
#include "mecz.h"

class Kosz : public Mecz {
public:
    Kosz(std::string nazwa, std::string opponent1, std::string opponent2, float kurs1, float kurs2);
    Kosz();
    ~Kosz() override;
    int get_needed_skill() const override { return 2; } // Koszykówka wymaga umiejętności koszykarskich
    std::string get_next_play() override;
};

#endif // KOSZ_H