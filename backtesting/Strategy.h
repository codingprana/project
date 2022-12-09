// Strategy.h
#include <vector>
#include <iostream>
#include <string>
#include <sstream> // for ostream

#ifndef STRATEGY_H
#define STRATEGY_H

// Abstract class
class Strategy {
public:
  enum class Signal{HOLD, BUY, SELL};
  friend std::ostream& operator<<(std::ostream& os, const Signal &s);
  
  // ctor & dtor
  Strategy();
  Strategy(double);
  Strategy(std::string);
  Strategy(std::string, double, double, double, int);
  virtual ~Strategy() = default;

  // read and store the close price 
  void read_data(std::string);
  void read_data(std::string, std::string);
  
  // getters
  double get_sharpe_ratio() const;
  double get_principal() const;
  double get_curr_profit() const;
  int get_curr_position() const;
  int get_data_size() const;

  // setters
  void set_sharpe_ratio(double);
  void set_principal(double);
  void set_curr_profit(double);
  void set_curr_position(int);
  void set_params(double, double, double, int);
  virtual void reset_strategy(double);

  // for trading
  void stop(int);
  void buy(int);
  void sell(int);
  void calculate_sharpe_ratio(double);

  // pure virtual functions
  virtual void update_portfolio(int, bool) = 0;
  virtual void calculate_portfolio(bool) = 0;
  virtual void print_title() = 0;

protected:
  std::vector<double> close_price; // store the close price
  std::vector<double> period_profit; // strategy profit in each period: 
                                     // every day, 10-mins, 5-mins, or 1-min

private:
  int curr_position; // declared int for pursose as we only want whole number
  double principal;
  double curr_profit; // this will be total profit in the end
  double sharpe_ratio;
};

#endif