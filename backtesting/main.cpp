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

void print_macd_params(const vector<double>& vec){
  cout << setw(14) << "Total Profit" << setw(14) << "Sharpe Ratio"
       << setw(14) << "MACD Fast" << setw(14) << "MACD Slow" << setw(14) << "MACD Preiod" << endl;
  for (const auto &parameter : vec) {
    cout << setw(14) << parameter;
  }
  cout << endl << endl;
}

void print_rsi_params(const vector<double>& vec){
  cout << setw(14) << "Total Profit" << setw(14) << "Sharpe Ratio"
       << setw(14) << "RSI period" << setw(14) << "RSI buy" << setw(14) << "RSI sell" << endl;
  for (const auto &parameter : vec) {
    cout << setw(14) << parameter;
  }
  cout << endl << endl;
}

int main() {
  cout << fixed << setprecision(2);
  cout << "\nOOP Project for Backtesting\n\n";
  cout << "Key in the file directory: ";
  string file_dir; // set the working directory where the data exists
  getline(cin, file_dir);

  // process file name and related parameters
  vector<string> time_interval{"BTC_data_daily.csv", "BTC_data_10-min.csv", "BTC_data_5-min.csv", "BTC_data_1-min.csv"};
  vector<double> sharpe_t{365.0, 52560.0, 105120.0, 525600.0}; // its basically annualization factor t
  const double fixed_principle{100000};
  RSI rsi_strat(fixed_principle);
  MACD macd_strat(fixed_principle);

  for (int i{0}; i < time_interval.size(); ++i){
    //RSI
    cout << "\nRSI strategy backtesting for " << time_interval[i] << ": \n\n";
    rsi_strat.read_data(time_interval[i], file_dir);
    vector<double> rsi_params{rsi_strat.grid_search(fixed_principle, sharpe_t[i], -0.5)};
    if(rsi_params.empty()){
      cout << "No params set has positive sharpe ratio" << endl;
    }
    else{
      print_rsi_params(rsi_params);
    }

    //MACD
    cout << "\nMACD strategy backtesting for " << time_interval[i] << ": \n\n";
    macd_strat.read_data(time_interval[i], file_dir);
    vector<double> macd_params{macd_strat.grid_search(10, 24, 24, 36, 9, 21, fixed_principle, sharpe_t[i], 0.0)};
    // uncomment 3 lines below to show the best portfolio simulation
    // MACD_port.set_principal(100000);
    // MACD_port.reset_params(macd_params[2], macd_params[3], macd_params[4]);
    // MACD_port.calculate_portfolio(true);
    if(macd_params.empty()){
      cout << "No params set has positive sharpe ratio" << endl;
    }
    else{
      print_macd_params(macd_params);
    }
  }

  return 0;
}


/*

g++ -c Strategy.cpp RSI.cpp MACD.cpp main.cpp -std=c++20
g++ Strategy.o RSI.o MACD.o main.o -o backtest_result

/Users/garrett/Documents/GitHub/project/backtesting
/Users/minghimlau/Desktop/backtesting
*/