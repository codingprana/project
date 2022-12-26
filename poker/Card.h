// Card.h
// define Card class in a No Limit Hold'em Game
#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>
#include <array>

class Card {
public:
    explicit Card(unsigned int = 0, unsigned int = 0);
    void setCard(unsigned int, unsigned int);
    unsigned int getSuit() const;
    unsigned int getValue() const;
    std::string toString() const;
private:
    unsigned int suit;
    unsigned int value;
    static const std::array<char, 5> suit_str;
    static const std::array<char, 14> value_char;
};

#endif