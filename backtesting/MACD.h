// MACD.h
// MACD class derived from class
// Strategy

#ifndef MACD_H
#define MACD_H

#include <vector>
#include "Strategy.h"

class MACD : public Strategy {
public:
  // ctor & dtor
  MACD();
  MACD(double);
  MACD(std::string);
  MACD(std::string, double, double, double, int, 
       unsigned int, unsigned int, unsigned int);
  virtual ~MACD() = default;
  
  // getters
  unsigned int get_macd_fast(); //set and get functions
  unsigned int get_macd_slow(); //set and get functions
  unsigned int get_macd_period(); //set and get functions

  // setters
  void set_macd_fast(unsigned int); //set and get functions
  void set_macd_slow(unsigned int); //set and get functions
  void set_macd_period(unsigned int); //set and get functions
  void set_MACD_params(unsigned int, unsigned int, unsigned int); //one stop reset
  virtual void reset_strategy(double) override;

  // for MACD strategy trading
  double calculate_moving_average(const std::vector<double>&, unsigned int, unsigned int) const; //MA
  void calculate_macd_line(unsigned int); //MACD line
  void calculate_signal_line(unsigned int); //Signal line
  Signal signal(double, double) const; //get trading signal
  virtual void update_portfolio(int, bool) override;
  virtual void calculate_portfolio(bool) override;
  virtual void print_title() override;
  void display(int, double, double, Signal) const;

  // for finding optimal strategy
  virtual std::vector<double> grid_search(unsigned int, unsigned int, unsigned int, 
                                          unsigned int, unsigned int, unsigned int,
                                          double, double, double);

private:
  unsigned int macd_fast, macd_slow, macd_period;
  std::vector<double> macd_line, signal_line;
};
#endif
