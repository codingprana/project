#include <iostream>
#include "Card.h"
#include "Game.h"
#include "Player.h"
using namespace std;

int main() {
    // random dealing testing
    Game game(6);
    //game.test_1();
    Player p1(Card(0, 0), Card(0, 0));
    //p1.test_2();
    int goal{20000}, count{goal};
    while(count--){
        cout << "#" << goal - count << "  "; 
        game.test_2();
    }
    


    return 0;
}

/*
g++ -c Player.cpp Game.cpp Card.cpp main.cpp -std=c++20
g++ Player.o Game.o Card.o main.o -o test
*/