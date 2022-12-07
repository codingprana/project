// Strategy.h
#include <vector>
#include <iostream>
#include <string>
#include <sstream> // for ostream

#ifndef STRATEGY_H
#define STRATEGY_H

//Abstract class
class Strategy {
public:
  enum class Signal{HOLD, BUY, SELL};
  friend std::ostream& operator<<(std::ostream& os, const Signal &s);
  
  //constructor
  Strategy(std::string, double);
  virtual ~Strategy() = default;

  void read_data(std::string);
  void print_close_price() const;
  int get_data_size() const;
  double get_sharpe_ratio() const;

  //
  void stop(int);
  void buy(int);
  void sell(int);
  virtual void update_portfolio(int, bool) = 0; //pure vitual
  virtual void print_title() = 0; //pure vitual
  virtual void calculate_portfolio(bool) = 0; //pure vitual
  void set_sharpe_ratio(double);
  void set_principal(double);
  void set_curr_profit(double);
  void set_curr_position(int);


protected:
  double principal{0};
  double curr_profit{0}; // this will be total profit in the end
  int curr_position{0}; //declared int for pursose as we only want whole number
  std::vector<double> close_price;
  std::vector<double> daily_profit;
  double sharpe_ratio{0};
};

#endif