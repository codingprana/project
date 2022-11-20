// main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include "Strategy.h"

using namespace std;

ostream& operator<<(ostream& os, const Strategy::Signal &s){
  if (s == Strategy::Signal::BUY){os << "BUY";}
  else if (s == Strategy::Signal::SELL){os << "SELL";}
  else if (s == Strategy::Signal::HOLD){os << "HOLD";}
  else{os << "This should never happens..";}
  return os;
}

int main() {
  Strategy RSI(100000, 25, 75, 40, 60, 14);
  RSI.read_data("BTC_data_daily.csv");
  for (int i{0}; i < RSI.get_data_size(); ++i){
    RSI.update_portfolio(i);
  }

  // Strategy RSI2(1000000, 30, 70, 50, 14);
  // //RSI2.read_data("BTC_data_10-min.csv");
  // //RSI2.print_close_price();

  // Strategy RSI3(1000000, 30, 70, 50, 14);
  // //RSI3.read_data("BTC_data_5-min.csv");
  // //RSI3.print_close_price();

  // Strategy RSI4(1000000, 30, 70, 50, 14);
  // //RSI4.read_data("BTC_data_1-min.csv");
  // //RSI4.print_close_price();

  return 0;
}