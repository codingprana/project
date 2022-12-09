// Strategy.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include "Strategy.h"

using namespace std;


// ctor
Strategy::Strategy() {
  set_params(0, 100000, 0, 0);
}

Strategy::Strategy(double prcpl) {
  set_params(0, prcpl, 0, 0);
}

Strategy::Strategy(string file_name) {
  set_params(0, 100000, 0, 0);
}

Strategy::Strategy(string file_name, double sharp_r, 
                   double prcpl, double profit, int pos) {
  read_data(file_name);
  set_params(sharp_r, prcpl, profit, pos);
}

void Strategy::read_data(string file_name) { 
  cout << "Key in the file directory: \n";
  string file_dir; // set the working directory where the data exists
  getline(cin, file_dir);
  read_data(file_name, file_dir);
}

void Strategy::read_data(string file_name, string file_dir) {
  // clear data if already exist data in the object
  if (!close_price.empty()) close_price.clear();
  
  ifstream data(file_dir + "/" + file_name, ios::in);
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


// getters
double Strategy::get_sharpe_ratio() const {
  return sharpe_ratio;
}

double Strategy::get_principal() const {
  return principal;
}

double Strategy::get_curr_profit() const {
  return curr_profit;
}

int Strategy::get_curr_position() const {
  return curr_position;
}

int Strategy::get_data_size() const {
  return close_price.size();
}


// setters
void Strategy::set_sharpe_ratio(double sharp_r)  {
  sharpe_ratio = sharp_r;
}

void Strategy::set_principal(double prcpl)  {
  principal = prcpl;
}

void Strategy::set_curr_profit(double profit)  {
  curr_profit = profit;
}

void Strategy::set_curr_position(int pos)  {
  curr_position = pos;
}

void Strategy::set_params(double sharp_r, double prcpl, double profit, int pos) {
  set_sharpe_ratio(sharp_r);
  set_principal(prcpl);
  set_curr_profit(profit);
  set_curr_position(pos);
}

void Strategy::reset_strategy(double prcpl) {
  set_params(0, prcpl, 0, 0);
  period_profit.clear();
}

// for trading
void Strategy::stop(int timeTick) {
  // update principal for the stop signal
  int pos{curr_position};
  curr_position = 0;
  principal -= (curr_position - pos)  * close_price[timeTick];
}

void Strategy::buy(int timeTick) {
  // update principal for the buy signal
  int pos{curr_position};
  if (curr_position == 0) {curr_position = max(0.0, principal) / close_price[timeTick];}
  else if (curr_position < 0) {
    stop(timeTick); 
    curr_position = max(0.0, principal) / close_price[timeTick];
    pos = 0;
  }
  principal -= (curr_position - pos) * close_price[timeTick];
}

void Strategy::sell(int timeTick) {
  // update principal for the sell signal
  int pos{curr_position};
  if (curr_position == 0) {curr_position = min(0.0, -principal) / close_price[timeTick];}
  else if (curr_position > 0) {
    stop(timeTick); 
    curr_position = min(0.0, -principal) / close_price[timeTick];
    pos = 0;
  }
  principal -= (curr_position - pos)  * close_price[timeTick];
}

void Strategy::calculate_sharpe_ratio(double t = 365) {
  // argument t = 365      (daily), 365*24*6 (10-min),
  //              365*24*14(5-min), 365*24*60(1-min)
  double mean{0}, var{0};
  for (const auto &n : period_profit) {
    mean += n;
  }
  mean /= period_profit.size();
  for (const auto &n : period_profit) {
    var += pow(n - mean, 2);
  }
  var /= (period_profit.size() - 1);

  sharpe_ratio = mean / sqrt(var) * sqrt(t);
}