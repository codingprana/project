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


//constructor
Strategy::Strategy(string file_name, double prcpl = 100000) 
  : principal{prcpl} {
  read_data(file_name);
}

void Strategy::read_data(string file_name) {
  //clear data if already exist data in the object
  if (close_price.empty() != true){close_price.clear();}
  
  cout << "Key in the working directory: \n";
  string wkdir; // set the working directory where the data exists
  getline(cin, wkdir);
  ifstream data(wkdir + "/" + file_name, ios::in);
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

//int division will truncate the decimal places and round the number down
void Strategy::stop(int timeTick) {
  int pos{curr_position}; // temp position
  curr_position = 0;
  principal -= (curr_position - pos)  * close_price[timeTick];
}

void Strategy::buy(int timeTick) {
  int pos{curr_position}; // temp position
  if (curr_position == 0) {curr_position = max(0.0, principal) / close_price[timeTick];}
  else if (curr_position < 0) {
    stop(timeTick); 
    curr_position = max(0.0, principal) / close_price[timeTick];
    pos = 0;
  }
  //it shud not be called if curr pos > 0
  principal -= (curr_position - pos) * close_price[timeTick];
}

void Strategy::sell(int timeTick) {
  int pos{curr_position}; // temp position
  if (curr_position == 0) {curr_position = min(0.0, -principal) / close_price[timeTick];}
  else if (curr_position > 0) {
    stop(timeTick); 
    curr_position = min(0.0, -principal) / close_price[timeTick];
    pos = 0;
  }
  //it shud not be called if curr pos < 0
  principal -= (curr_position - pos)  * close_price[timeTick];
}

double Strategy::get_sharpe_ratio() const {
  return sharpe_ratio;
}

void Strategy::set_sharpe_ratio(double t = 365) {
  double mean{0}, var{0};
  for (const auto &n : daily_profit) {
    mean += n;
  }
  mean /= daily_profit.size();
  for (const auto &n : daily_profit) {
    var += pow(n - mean, 2);
  }
  var /= (daily_profit.size() - 1);

  sharpe_ratio = mean / sqrt(var) * sqrt(t);
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