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

//should have make this a member function
void print_macd_params(string title, const vector<double>& vec){
  cout << endl << title << endl;
  cout << setw(14) << "Total Profit" << setw(14) << "Sharpe Ratio"
       << setw(14) << "MACD Fast" << setw(14) << "MACD Slow" << setw(14) << "MACD Preiod" << endl;
  for (const auto &parameter : vec) {
    cout << setw(14) << parameter;
  }
  cout << endl;
}

void print_rsi_params(string title, const vector<double>& vec){
  cout << endl << title << endl;
  cout << setw(14) << "Total Profit" << setw(14) << "Sharpe Ratio"
       << setw(14) << "RSI period" << setw(14) << "RSI buy" << setw(14) << "RSI sell" << endl;
  for (const auto &parameter : vec) {
    cout << setw(14) << parameter;
  }
  cout << endl;
}

int main() {
  //driver program maybe make a better interface if we have more time
  cout << fixed << setprecision(2);

  //Stupid way I guess
  vector<string> time_interval{"BTC_data_daily.csv", "BTC_data_10-min.csv", "BTC_data_5-min.csv", "BTC_data_1-min.csv"};
  vector<double> sharpe_t{365.0, 52560.0, 105120.0, 525600.0}; // its basically annualization factor t
  RSI rsi_strat("BTC_data_daily.csv", 100000, 30, 70, 50, 50, 14);
  MACD macd_strat("BTC_data_daily.csv", 100000, 12, 26, 9);

  for (int i{0}; i < time_interval.size(); ++i){
    //RSI
    rsi_strat.read_data(time_interval[i]);
    vector<double> rsi_params{rsi_strat.grid_search(100000, sharpe_t[i], -0.5)};
    if(rsi_params.empty() == false){
      print_rsi_params(time_interval[i], rsi_params);
    }
    else{cout << "No params set has positive sharpe ratio" << endl;}

    //MACD
    macd_strat.read_data(time_interval[i]);
    vector<double> macd_params{macd_strat.grid_search(10, 24, 24, 36, 9, 21, 100000, sharpe_t[i], 0.0)};
    // uncomment 3 lines below to show the best portfolio simulation
    // MACD_port.set_principal(100000);
    // MACD_port.reset_params(macd_params[2], macd_params[3], macd_params[4]);
    // MACD_port.calculate_portfolio(true);
    if(macd_params.empty() == false){
      print_macd_params(time_interval[i], macd_params);
    }
    else{cout << "No params set has positive sharpe ratio" << endl;}
  }

  return 0;
}


/*

g++ -c Strategy.cpp RSI.cpp main.cpp -std=c++20
g++ Strategy.o RSI.o main.o -o backtest_result

/Users/garrett/Documents/GitHub/project/backtesting
/Users/minghimlau/Desktop/backtesting
*/