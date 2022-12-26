// Game.h
// define a class for no limit hold'em game
#ifndef GAME_H
#define GAME_H

#include <array>
#include <vector>
#include <map>
#include <utility>
#include "Player.h"
#include "Card.h"

class Game {
public:
    explicit Game(unsigned int = 6);
    void resetPlayerHoleCards();
    void resetPlayerHands();
    void build_a_deck();
    void setPlayerNumber(unsigned int);
    void new_game();
    void updatePlayerHands();// 

    unsigned int getPlayerNumber() const;
    std::vector<std::vector<unsigned int>> getPlayerHands() const;
    void info() const;
    void printDeck() const;

    // test functions
    void test_1();
    void test_2();

private:
    std::array<unsigned int, 52> deck;
    std::array<std::array<unsigned int, 2>, 52> deck2;
    unsigned int playerNumber;
    std::array<Card, 5> communityCards;
    std::vector<std::array<Card, 2>> playerHoleCards;
    std::vector<std::vector<unsigned int>> playerHands;

    static const std::array<std::string, 11> hand_rankings;
    static const std::array<unsigned int, 14> valuesLeftShift;
    static const std::array<unsigned int, 14> valuesRightShift;
    static std::map<unsigned int, char> toPokerValue;

    // utility functions
    unsigned int getSuitFromDeck(unsigned int n) const;
    unsigned int getValueFromDeck(unsigned int n) const;
    void shuffle_cards();
    void deal_cards();
    std::vector<unsigned int> player_hand(unsigned int) const;
    std::vector<std::vector<unsigned int>> player_cards(unsigned int) const;
    std::vector<unsigned int> checkFlush(const std::vector<std::vector<unsigned int>>&, 
                                         std::vector<unsigned int>&) const;
    void checkStraight(std::vector<unsigned int>&, std::vector<unsigned int>&) const;
    void checkNoStraight (std::vector<unsigned int>&, std::vector<unsigned int>&) const;
    std::pair<unsigned int, unsigned int> getMaxCount(std::map<unsigned int, unsigned int>& m) const;
};

#endif