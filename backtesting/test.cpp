#include <iostream>
#include "Strategy.h"


using namespace std;



int main() {
  Strategy RSI(100000, 30, 70, 40, 60, 7);
  RSI.read_data("BTC_data_daily.csv");
  for (int i{0}; i < RSI.get_data_size(); ++i){
    RSI.update_portfolio(i);
  }
  

  // Strategy RSI2(100000, 25, 75, 40, 60, 14);
  // RSI2.read_data("BTC_data_10-min.csv");
  // for (int i{0}; i < RSI2.get_data_size(); ++i){
  //   RSI2.update_portfolio(i);
  // }

  // Strategy RSI3(100000, 25, 75, 40, 60, 14);
  // RSI3.read_data("BTC_data_5-min.csv");
  // for (int i{0}; i < RSI3.get_data_size(); ++i){
  //   RSI3.update_portfolio(i);
  // }

  // Strategy RSI4(100000, 25, 75, 40, 60, 14);
  // RSI4.read_data("BTC_data_1-min.csv");
  // for (int i{0}; i < 1000; ++i){
  //   RSI4.update_portfolio(i);
  // }


  return 0;
}