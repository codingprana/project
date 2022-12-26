// Player.cpp
#include <iostream>
#include "Player.h"
using namespace std;

Player::Player(Card card1, Card card2) 
    : holeCard_1{card1}, holeCard_2{card2} {};

void Player::setHoleCard_1(unsigned int suit, unsigned int value) {
    holeCard_1.setCard(suit, value);
}

void Player::setHoleCard_2(unsigned int suit, unsigned int value) {
    holeCard_2.setCard(suit, value);
}

Card Player::getHoleCard_1() const {
    return holeCard_1;
}

Card Player::getHoleCard_2() const {
    return holeCard_2;
}

void Player::test_2() {
    cout << "Test 2:\n" << endl;
    cout << getHoleCard_1().toString() << " ";
    cout << getHoleCard_2().toString() << endl;
    setHoleCard_1(2, 3);
    setHoleCard_2(4, 1);
    cout << getHoleCard_1().toString() << " ";
    cout << getHoleCard_2().toString() << endl;
}