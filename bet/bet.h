#ifndef BET_H
#define BET_H
#include <string>
#include <vector>

class Bet {
private:
    int BID;
    // 0 - noone, 1 - opponent1, 2 - opponent2
    int who;
    int amount;
    
public:
    Bet(int BID, int who, int amount);
    int getBID() const;
    int getWho() const;
    int getAmount() const;
    void setWho(int who);
    void setAmount(int amount);
};


#endif
