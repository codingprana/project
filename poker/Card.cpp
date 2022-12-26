// Card.cpp
#include <iostream>
#include <sstream>
#include <string>
#include "Card.h"
using namespace std;

const array<char, 5> Card::suit_str {
    '\0', 'c', 'd', 'h', 's'
};

const array<char, 14> Card::value_char {
    '\0', 'A', '2', '3', '4', '5', '6',
    '7', '8', '9', 'T', 'J', 'Q', 'K'
};

Card::Card(unsigned int s, unsigned int v)
    : suit{s}, value{v} {}

void Card::setCard(unsigned int s, unsigned int v) {
    suit = s; value = v;
}

unsigned int Card::getSuit() const {
    return suit;
}

unsigned int Card::getValue() const {
    return value;
}

string Card::toString() const {
    ostringstream output;
    output << suit_str[getSuit()] << value_char[getValue()];
    return output.str();
}
