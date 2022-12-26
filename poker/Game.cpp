//Game.cpp
#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>
#include <algorithm>
#include <vector>
#include <map>
#include <utility>
#include "Game.h"
using namespace std;

default_random_engine engine{static_cast<unsigned int>(time(0))};
uniform_int_distribution<unsigned int> randomInt{0, 4294967295};

const array<string, 11> Game::hand_rankings {
    "", "High Card", "One Pair", "Two Pairs", "Trips", "Straight",
    "Flush", "Full House", "Quads", "Straight Flush", "Royal Flush"
};

map<unsigned int, char> Game::toPokerValue {
    {1, '2'}, {2, '3'}, {3, '4'}, {4, '5'}, 
    {5, '6'}, {6, '7'}, {7, '8'}, {8, '9'}, 
    {9, 'T'}, {10, 'J'}, {11, 'Q'}, {12, 'K'}, {13, 'A'}
};

const array<unsigned int, 14> Game::valuesLeftShift {
    0, 13, 1, 2, 3, 4, 5, 
    6, 7, 8, 9, 10, 11, 12
};

const array<unsigned int, 14> Game::valuesRightShift {
    0, 2, 3, 4, 5, 6, 7, 8,
    9, 10, 11, 12, 13, 1
};

Game::Game(unsigned int n) {
    setPlayerNumber(n);
    build_a_deck();
    new_game();
}

void Game::resetPlayerHoleCards() {
    if (!playerHoleCards.empty()) {
        playerHoleCards.clear();
    }
    for (int i{0}; i < getPlayerNumber(); ++i) {
        playerHoleCards.push_back({Card(), Card()});
    }
}

void Game::resetPlayerHands() {
    if (!playerHands.empty()) {
        playerHands.clear();
    }
    for (int i{0}; i < getPlayerNumber(); ++i) {
        playerHands.push_back({1, 0, 0, 0, 0, 0});
    }
}

void Game::build_a_deck() {
    // /// deck
    // unsigned int value{0};
    // for (auto& card_code : deck) {
    //     card_code = value;
    //     ++value;
    // }
    // deck2
    unsigned int value{0};
    for (auto& card_code : deck2) {
        card_code[1] = value;
        ++value;
    }
}

unsigned int Game::getSuitFromDeck(unsigned int n) const {
    return 1 + n / 13;
}


unsigned int Game::getValueFromDeck(unsigned int n) const {
    return 1 + n % 13;
}

void Game::setPlayerNumber(unsigned int n) {
    playerNumber = n;
    resetPlayerHoleCards();
    resetPlayerHands();
}

void Game::new_game() {
    shuffle_cards();
    deal_cards();
}

void Game::shuffle_cards() {
    // // deck
    // shuffle( deck.begin(), deck.end(), 
    //          default_random_engine( static_cast<unsigned int>(time(0)) ) );
    
    // deck2 
    for (auto& card : deck2) {
        card[0] = randomInt(engine);
    }
    sort(deck2.begin(), deck2.end());
}

void Game::deal_cards() {
    unsigned int n{getPlayerNumber()};
    // // deck
    // // deal player hole cards
    // for (int i{0}; i < n; ++i) {
    //     playerHoleCards[i][0].setCard(getSuitFromDeck(deck[i]), getValueFromDeck(deck[i]));
    //     playerHoleCards[i][1].setCard(getSuitFromDeck(deck[i+n]), getValueFromDeck(deck[i+n]));
    // }
    // // deal community cards
    // for (unsigned int i{2*n}, count{0}; count < 5; ++i, ++count) {
    //     communityCards[count].setCard(getSuitFromDeck(deck[i]), getValueFromDeck(deck[i]));
    // }

    // deck2
    // deal player hole cards
    for (int i{0}; i < n; ++i) {
        playerHoleCards[i][0].setCard(getSuitFromDeck(deck2[i][1]), getValueFromDeck(deck2[i][1]));
        playerHoleCards[i][1].setCard(getSuitFromDeck(deck2[i+n][1]), getValueFromDeck(deck2[i+n][1]));
    }
    // deal community cards
    for (unsigned int i{2*n}, count{0}; count < 5; ++i, ++count) {
        communityCards[count].setCard(getSuitFromDeck(deck2[i][1]), getValueFromDeck(deck2[i][1]));
    }
}



unsigned int Game::getPlayerNumber() const {
    return playerNumber;
}

vector<vector<unsigned int>> Game::getPlayerHands() const {
    return playerHands;
}

void Game::info() const {
    cout << "Community Cards: \n";
    for (const auto& card: communityCards) {
        cout << card.toString() << " ";
    }
    cout << endl << endl;

    cout << "Player Hole Cards: \n";
    int count{1};
    for (const auto& hole_cards: playerHoleCards) {
        cout << "Player-" << count << ": ";
        cout << hole_cards[0].toString() << " "
             << hole_cards[1].toString() << "\n";
        ++count;
    }
    cout << endl;
}

void Game::printDeck() const {
    cout << "The whole Deck: \n";
    int count{1};
    for (const auto& card : deck) {
        cout << count << ": " << card << "\n";
        ++count;
    }
    cout << endl;
}


void Game::test_1() {
    cout << "Test 1: \n" << endl;
    srand(time(0));
    int count{0};
    while(++count && count <= 20) {
        cout << "Game " << count << endl;
        setPlayerNumber(1 + rand()%9);
        new_game();
        info();
        printDeck();
    }
}

void Game::updatePlayerHands() {
    for (unsigned int i{0}; i < getPlayerNumber(); ++i) {
        playerHands[i] = player_hand(i);
    }
}

vector<unsigned int> Game::player_hand(unsigned int i) const { // i(player's index): 0-> 1st, 1-> 2nd, ...
    // player_hand example:
    // {2, 13, 13, 12, 2, 1} --> {one pair, 1, 1, 12, 3, 2}
    // {10, 13, 12, 11, 10, 9} --> {royal flush, 1, 13, 12, 11, 10}
    vector<unsigned int> hand{1}; // At front add hand_rankings 1: High Card
    vector<vector<unsigned int>> cards{player_cards(i)};

    // flush check
    vector<unsigned int> values{checkFlush(cards, hand)};
    
    // straight check
    checkStraight(values, hand);
    if (1 != hand[0]) return hand;

    // no-straight check
    checkNoStraight(values, hand);
    return hand;
}

vector<vector<unsigned int>> Game::player_cards(unsigned int i) const {
    vector<vector<unsigned int>> cards; // { {1, 2}, {2,3}, {4, 2}, ... }
    cards.push_back({playerHoleCards[i][0].getSuit(), valuesLeftShift[playerHoleCards[i][0].getValue()]});
    cards.push_back({playerHoleCards[i][1].getSuit(), valuesLeftShift[playerHoleCards[i][1].getValue()]});
    for (const auto& card : communityCards) {
        cards.push_back({card.getSuit(), valuesLeftShift[card.getValue()]});
    }
    sort(cards.begin(), cards.end(), greater());
    return cards;
}

vector<unsigned int> Game::checkFlush(const vector<vector<unsigned int>>& cards, 
                                      vector<unsigned int>& hand) const {
    vector<unsigned int> values;
    // if there is a flush
    for (int i{0}; i < cards.size()-4; ++i) {
        if (cards[i][0] == cards[4+i][0]) {
            hand[0] = 6; // At front assign hand_rankings 6: flush
            for (int j{i}; j < 5 + i; ++j) {
                values.push_back(cards[j][1]);
            }
            int j{4+i};
            while (++j < cards.size()) {
                if (cards[i][0] == cards[j][0]) {
                    values.push_back(cards[j][1]);
                }
            }
            return values;
        }
    }
    // if there is no flush
    for (const auto& card : cards) {
        values.push_back(card[1]);
    }
    sort(values.begin(), values.end(), greater());
    return values;
}

void Game::checkStraight(vector<unsigned int>& values, vector<unsigned int>& hand) const {
    // build value set to check if any straight exicts
    vector<unsigned int> valueSet{values[0]};
    for (int i{1}; i < values.size(); ++i) {
        if (values[i] != values[i-1]) {
            valueSet.push_back(values[i]);
        }
    }
    // (1) no straight, no flush -> futher check no straight hands
    if (5 > valueSet.size()) {
        return;
    }
    // check straight
    // (2) check straight form: 13-9, ..., 5-1 (13, 4-1 later)
    for (int i{0}; i < valueSet.size() - 4; ++i) {
        if (valueSet[i] == 4 + valueSet[4+i]) {
            // update player's hand if there is a straight
            for (int j{i}; j < 5 + i; ++j) {
                hand.push_back(valueSet[j]);
            }
            //   hand_ranking = hand[0]
            //         flush  = 6  
            //    royal flush = 10
            // straight flush = 9
            //       straight = 5
            if (13 == valueSet[i]) { hand[0] = (6 == hand[0] ? 10 : 5); } 
            else { hand[0] = (6 == hand[0] ? 9 : 5); }
            return;
        }
    }
    // (3) check straight form: 13, 4-1
    if (13 == valueSet[0] && 1 == valueSet.back() && 4 == valueSet[valueSet.size()-4]) {
        // update player's hand if there is a straight
        hand.push_back(valueSet[0]);
        for (int i = valueSet.size()-4; i < valueSet.size(); ++i) {
            hand.push_back(valueSet[i]);
        }
        hand[0] = (6 == hand[0] ? 9 : 5);
        return;
    }
    // (4) no straight, flush
    if (6 == hand[0]) {
        for (int i{0}; i < 5; ++i) {
            hand.push_back(values[i]);
        }
        return;
    }
    
}

void Game::checkNoStraight(vector<unsigned int>& values, vector<unsigned int>& hand) const {
    // build value map to count frequency
    map<unsigned int, unsigned int> m;
    for (const auto& value : values) {
        ++m[value];
    }
    // get max count pair
    pair<unsigned int, unsigned int> p(getMaxCount(m)); //p.first : max count value
                                                        //p.second: max count

    // Classification
    // High Card (default hand[0] = 1)
    if (1 == p.second) {
        for (int i{0}; i < 5; ++i) {
            hand.push_back(values[i]);
        }
        return;
    }
    // append max count value (count >= 2)
    for (int i{0}; i < p.second; ++i) {
        hand.push_back(p.first);
    }
    // one pair(2), two pairs(3)
    if (2 == p.second) {
        m.erase(p.first);
        p = getMaxCount(m);
        if (1 == p.second) {
            //one pair(2)
            hand[0] = 2;
            hand.push_back((--m.end())->first);
            for (int i{0}; i < 2; ++i) {
                m.erase((--m.end())->first);
                hand.push_back((--m.end())->first);
            }
            return;
        }
        else {
            // two pairs(3)
            hand[0] = 3;
            hand.push_back(p.first);
            hand.push_back(p.first);
            m.erase(p.first);
            hand.push_back((--m.end())->first);
            return;
        }
    }
    // trips(4), full-house(7)
    if (3 == p.second) {
        m.erase(p.first);
        p = getMaxCount(m);
        if (1 == p.second) {
            // trips(4)
            hand[0] = 4;
            hand.push_back((--m.end())->first);
            m.erase((--m.end())->first);
            hand.push_back((--m.end())->first);
            return;
        }
        else {
            // full-house(7)
            hand[0] = 7;
            hand.push_back(p.first);
            hand.push_back(p.first);
            return;
        }
    }
    // Quads(8)
    if (4 == p.second) {
        hand[0] = 8;
        m.erase(p.first);
        hand.push_back((--m.end())->first);
        return;
    }    
}

void Game::test_2() {
    cout << "Test 2: print out each player's hand \n" << endl;
    new_game();
    updatePlayerHands();
    info();
    for (unsigned int i{0}; i < getPlayerNumber(); ++i) {
        cout << "\nPlayer-" << i+1 << ": ";

        cout << setw(15) << hand_rankings[playerHands[i][0]] << " ";
        for (unsigned int j{1}; j < playerHands[i].size(); ++j) {
            cout << setw(2) << toPokerValue[playerHands[i][j]] << " ";
            //cout << setw(2) << valuesRightShift[Hands[i][j]] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

pair<unsigned int, unsigned int> Game::getMaxCount(map<unsigned int, unsigned int>& m) const {
    pair<unsigned int, unsigned int> p(0, 0);
    map<unsigned int, unsigned int>::iterator itr;
    for (itr = m.begin(); itr != m.end(); ++itr) {
        p.second = max(p.second, itr->second);
        if (p.second == itr->second) {
            p.first = itr->first;
        }
    }
    return p;
}
