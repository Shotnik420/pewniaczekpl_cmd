#include "bet.h"

Bet::Bet(int BID, int who, int amount) : BID(BID), who(who), amount(amount) {}
Bet::Bet() : BID(0), who(0), amount(0) {}
int Bet::getBID() const {
    return BID;
}
int Bet::getWho() const {
    return who;
}
int Bet::getAmount() const {
    return amount;
}
void Bet::setWho(int who) {
    this->who = who;
}
void Bet::setAmount(int amount) {
    this->amount = amount;
}