// main.cpp
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Strategy.h"
#include "RSI.h"
#include "MACD.h"

using namespace std;

ostream& operator<<(ostream& os, const Strategy::Signal &s){
  if (s == Strategy::Signal::BUY){os << "BUY";}
  else if (s == Strategy::Signal::SELL){os << "SELL";}
  else if (s == Strategy::Signal::HOLD){os << "HOLD";}
  else{os << "This should never happen...";}
  return os;
}

int main() {
  // RSI RSI1("BTC_data_daily.csv", 100000, 23, 62, 50, 50, 21);
  // vector<double> v;
  // v = RSI1.grid_search(100000, 365, -0.5);
  // RSI1.set_principal(100000);
  // RSI1.set_params(v[3], v[4], 50, 50, v[2]);
  // cout << fixed << setprecision(2);
  // RSI1.calculate_portfolio(true);
  // cout << endl;
  // cout << setw(14) << "Total Profit" << setw(14) << "Sharpe Ratio"
  //      << setw(14) << "RSI Period" << setw(14) << "Buy" << setw(14) << "Sell" << endl;
  // for (const auto &parameter : v) {
  //   cout << setw(14) << parameter;
  // }
  // cout << endl;


  // RSI RSI2("BTC_data_10-min.csv", 100000, 23, 62, 50, 50, 21);
  // vector<double> v;
  // v = RSI2.grid_search(100000, 365*24*6, -0.5);
  // // for (int i{0}, end{RSI2.get_data_size()}; i < end; ++i ) {
  // //   RSI2.update_portfolio(i);
  // // }
  // cout << "total profit\t shapre ratio\t peroid\t buy\t sell\t " << endl;
  // for (const auto &parameter : v) {
  //   cout << parameter << "\t ";
  // }
  // cout << endl;


  // RSI RSI3("BTC_data_5-min.csv", 100000, 25, 75, 40, 60, 14);
  // vector<double> v;
  // v = RSI3.grid_search(100000, 365*24*12, 0);
  // cout << "total profit\t shapre ratio\t peroid\t buy\t sell\t " << endl;
  // for (const auto &parameter : v) {
  //   cout << parameter << "\t ";
  // }
  // cout << endl;


  // RSI RSI4("BTC_data_1-min.csv", 100000, 25, 75, 40, 60, 14);
  // vector<double> v;
  // v = RSI4.grid_search(100000, 365*24*60, 0);
  // cout << "total profit\t shapre ratio\t peroid\t buy\t sell\t " << endl;
  // for (const auto &parameter : v) {
  //   cout << parameter << "\t ";
  // }
  // cout << endl;
  MACD MACD1("BTC_data_daily.csv", 100000, 12, 26, 9);
  vector<double> v;
  v = MACD1.grid_search(10, 24, 24, 36, 9, 21, 100000, 365, 0.0);
  MACD1.set_principal(100000);
  MACD1.reset_params(v[2], v[3], v[4]);
  cout << fixed << setprecision(2);
  MACD1.calculate_portfolio(true);
  cout << endl;
  cout << setw(14) << "Total Profit" << setw(14) << "Sharpe Ratio"
       << setw(14) << "MACD Fast" << setw(14) << "MACD Slow" << setw(14) << "MACD Preiod" << endl;
  for (const auto &parameter : v) {
    cout << setw(14) << parameter;
  }
  cout << endl;

  return 0;
}


/*

g++ -c Strategy.cpp RSI.cpp main.cpp -std=c++20
g++ Strategy.o RSI.o main.o -o backtest_result

/Users/garrett/Documents/GitHub/project/backtesting
/Users/minghimlau/Desktop/backtesting
*/