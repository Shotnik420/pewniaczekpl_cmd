#pragma once

class Bet {
public:
    Bet() : bet(0) {}

    void set_bet(int value) {
        bet = value;
    }

    int get_bet() const {
        return bet;
    }

    ~Bet() = default;
private:
    int bet;
};