// main.cpp
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include "Strategy.h"
#include "RSI.h"


using namespace std;

ostream& operator<<(ostream& os, const Strategy::Signal &s){
  if (s == Strategy::Signal::BUY){os << "BUY";}
  else if (s == Strategy::Signal::SELL){os << "SELL";}
  else if (s == Strategy::Signal::HOLD){os << "HOLD";}
  else{os << "This should never happens..";}
  return os;
}


int main() {
  RSI RSI1("BTC_data_daily.csv", 100000, 23, 62, 50, 50, 21);
  vector<double> v;
  v = RSI1.grid_search(100000, 365, -0.5);
  RSI1.set_principle(100000);
  RSI1.set_params(v[3], v[4], 50, 50, v[2]);
  cout << fixed << setprecision(2);
  cout << setw(14) << "Time" << setw(14) << "Close" << setw(14) << "RSI"
       << setw(14) << "Signal" << setw(14) << "Principal"
       << setw(14) << "Position" << setw(14) << "Curr. Profit"
       << setw(14) << "Total Profit" << endl;
  for (int timeTick{0}, end{RSI1.get_data_size()}; timeTick < end; ++timeTick ) {
    RSI1.update_portfolio(timeTick, true);
  }
  cout << endl;
  cout << setw(14) << "Total Profit" << setw(14) << "Sharpe Ratio"
       << setw(14) << "RSI Period" << setw(14) << "Buy" << setw(14) << "Sell" << endl;
  for (const auto &parameter : v) {
    cout << setw(14) << parameter;
  }
  cout << endl;


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

  return 0;
}


/*

g++ -c Strategy.cpp RSI.cpp main.cpp -std=c++20
g++ Strategy.o RSI.o main.o -o backtest_result

/Users/garrett/Documents/GitHub/project/backtesting

*/