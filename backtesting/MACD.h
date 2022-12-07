//MACD.h
//MACD class derived from class
//Strategy

#ifndef MACD_H
#define MACD_H

#include <vector>
#include "Strategy.h"

class MACD : public Strategy {
public:
  MACD(std::string, double, unsigned int, unsigned int, unsigned int);
  virtual ~MACD() = default;
  void set_macd_fast(unsigned int); //set and get functions
  void set_macd_slow(unsigned int); //set and get functions
  void set_macd_period(unsigned int); //set and get functions
  void reset_params(unsigned int, unsigned int, unsigned int); //one stop reset
  unsigned int get_macd_fast(); //set and get functions
  unsigned int get_macd_slow(); //set and get functions
  unsigned int get_macd_period(); //set and get functions
  double calculate_moving_average(const std::vector<double>&, unsigned int, unsigned int) const; //MA
  void calculate_macd_line(unsigned int); //MACD line
  void calculate_signal_line(unsigned int); //Signal line
  virtual Signal signal(double, double) const; //get trading signal
  virtual void update_portfolio(int, bool) override; // all below are similar to RSI and Strategy classes
  virtual void print_title() override;
  virtual void calculate_portfolio(bool) override;
  virtual void display(int, double, double, Signal) const;
  virtual std::vector<double> grid_search(unsigned int, unsigned int, unsigned int, 
                                          unsigned int, unsigned int, unsigned int,
                                          double, double, double);

protected:
  unsigned int macd_fast{12}, macd_slow{26}, macd_period{9};
  std::vector<double> macd_line, signal_line;
};
#endif
