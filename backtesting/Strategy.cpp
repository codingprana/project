// Strategy.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "Strategy.h"

using namespace std;

//enum class Signal{HOLD, BUY, SELL};
//friend ostream& operator<<(ostream& os, const Signal &s);

//constructor
Strategy::Strategy(double prcpl, double buy, double sell, 
                   double buyStop, double sellStop, unsigned int period) 
    : principal{prcpl}
    , buySign{buy}
    , sellSign{sell}
    , buyStopSign{buyStop}
    , sellStopSign{sellStop}
    , rsi_period{period} 
{}

void Strategy::read_data(string file_name) {
  ifstream data("/Users/garrett/Documents/GitHub/project/backtesting/" + file_name, ios::in);
  if (!data) {
    cerr << "File could not be opened" << endl;
    exit(EXIT_FAILURE);
  }
  string str;
  getline (data, str);
  while (getline(data, str)) {
    istringstream iss(str);
    string token;
    getline(iss, token, ',');
    getline(iss, token, ',');
    getline(iss, token, ',');
    getline(iss, token, ',');
    getline(iss, token, ',');
    try {
      close_price.push_back(stod(token));
    }
    catch (invalid_argument) {
      cout << "Invalid argument";
    }
  }
}

void Strategy::print_close_price() const {
  unsigned int count{1};
  for (const double &i : close_price) {
    cout << count << ": " << i << endl;
    ++count;
  }
}

int Strategy::get_data_size() const {
  return close_price.size();
}

double Strategy::calculate_rsi(unsigned int timeTick = 0) const {
  static double rsi{0}, ave_gain{0}, ave_loss{0};
  static int count{-1};

  if (timeTick == 0) { // initialize for signal function to iterate
    count = -1;
    ave_gain = 0;
    ave_loss = 0;
  }
  if (count + 1 == timeTick) { // make sure to iterate with time tick one by one
    
    // insufficient periods to calculate RSI
    if (timeTick < rsi_period) {
      ++count;
      return -1;
    }

    // calculate first RSI
    if (timeTick == rsi_period) {
      for (unsigned int i{0}; i < rsi_period; ++i) {
        double dif{close_price[i+1] - close_price[i]};
        ave_gain += ( dif > 0 ? dif : 0);
        ave_loss += ( dif < 0 ? -dif : 0);
      }
      ave_gain /= rsi_period;
      ave_loss /= rsi_period;
      rsi = 100.0 - 100.0 / (1 + ave_gain / ave_loss);
      ++count;
      return rsi;
    }

    // calculate RSI other than the first one
    double dif{close_price[timeTick] - close_price[timeTick-1]};
    ave_gain = (ave_gain * (rsi_period - 1) + ( dif > 0 ? dif : 0) ) / rsi_period;
    ave_loss = (ave_loss * (rsi_period - 1) + ( dif < 0 ? -dif : 0) ) / rsi_period;
    rsi = 100.0 - 100.0 / (1 + ave_gain / ave_loss);
    ++count;
    return rsi;
  }
  else if (count == timeTick) return rsi; // call RSI without modifying
  else return -10; // Not iterate with time tick one by one

}

//int division will truncate the decimal places and round the number down
void Strategy::stop(int timeTick) {
  int pos{curr_position}; // temp position
  curr_position = 0;
  principal -= (curr_position - pos)  * close_price[timeTick];
}

void Strategy::buy(int timeTick) {
  int pos{curr_position}; // temp position
  if (curr_position == 0) {curr_position = max(0.0, principal) / close_price[timeTick];}
  else if (curr_position < 0) {stop(timeTick); curr_position = max(0.0, principal) / close_price[timeTick];}
  //it shud not be called if curr pos > 0
  principal -= (curr_position - pos) * close_price[timeTick];
}

void Strategy::sell(int timeTick) {
  int pos{curr_position}; // temp position
  if (curr_position == 0) {curr_position = min(0.0, -principal) / close_price[timeTick];}
  else if (curr_position > 0) {stop(timeTick); curr_position = min(0.0, -principal) / close_price[timeTick];}
  //it shud not be called if curr pos < 0
  principal -= (curr_position - pos)  * close_price[timeTick];
}


Strategy::Signal Strategy::rsi_signal(double rsi) const {
  //base base when rsi is out of range (not enough data point)
  if (rsi < 0) {return Signal::HOLD;}
  //current position is 0
  if (curr_position == 0){
    if (rsi > sellSign) {return Signal::SELL;}
    else if (rsi < buySign) {return Signal::BUY;}
    else {return Signal::HOLD;}
  }
  //current position is sell
  else if (curr_position < 0){
    if (rsi <= sellStopSign && buySign <= rsi) {return Signal::HOLD;}
    else if (rsi < buySign) {return Signal::BUY;}
    else {return Signal::SELL;}
  }
  //current position is buy can just use else
  else{
    if (rsi >= buyStopSign && sellSign >= rsi) {return Signal::HOLD;}
    else if (rsi > sellSign) {return Signal::SELL;}
    else {return Signal::BUY;}
  }
}

void Strategy::update_portfolio(int timeTick) {
  // double curr_principal = principal;
  double tempPortfolioValue{principal + curr_position * close_price[max(0, timeTick - 1)]};
  double curr_rsi = calculate_rsi(timeTick);
  Signal signal = rsi_signal(curr_rsi);

  //update position
  if (curr_position == 0){
    if (signal == Signal::BUY){buy(timeTick);}
    else if (signal == Signal::SELL){sell(timeTick);}
  }
  else if (curr_position < 0){
    if (signal == Signal::BUY){buy(timeTick);}
    else if (signal == Signal::HOLD){stop(timeTick);}
  }
  else if (curr_position > 0){
    if (signal == Signal::SELL){sell(timeTick);}
    else if (signal == Signal::HOLD){stop(timeTick);}
  }

  double newPortfolioValue{principal + curr_position * close_price[timeTick]};
  daily_profit.push_back(newPortfolioValue - tempPortfolioValue);
  curr_profit += daily_profit[timeTick];
  //just some debug print
  cout << timeTick << "\t" << close_price[timeTick] << "\t" << curr_rsi << "\t" << signal << "\t"; 
  cout << principal << "\t" << curr_position << "\t" << daily_profit[timeTick] << "\t" << curr_profit << endl;
}
