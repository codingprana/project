// Player.h
// define Player class in a No Limit Hold'em Game
#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"

class Player {
public:
    Player(Card = Card(), Card = Card());
    void setHoleCard_1(unsigned int, unsigned int);
    void setHoleCard_2(unsigned int, unsigned int);
    Card getHoleCard_1() const;
    Card getHoleCard_2() const;

    // test functions
    void test_2();
private:
    Card holeCard_1, holeCard_2;
};

#endif