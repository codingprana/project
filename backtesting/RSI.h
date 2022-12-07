//RSI.h
//RSI class derived from class
//Strategy

#ifndef RSI_H
#define RSI_H

#include <vector>
#include "Strategy.h"

class RSI : public Strategy {
public:
  RSI(std::string, double, double, double, double, double, unsigned int);
  virtual ~RSI() = default;
  double calculate_rsi(unsigned int) const;
  virtual Signal signal(double) const;
  virtual void update_portfolio(int, bool) override;
  virtual void display(int, double, Signal) const;
  virtual void print_title() override;
  virtual void calculate_portfolio(bool) override;
  virtual void set_params(double, double, double, double, unsigned int);
  virtual std::vector<double> grid_search(double, double, double);

protected:
  double buySign{0}, sellSign{0}, buyStopSign{0}, sellStopSign{0};
  unsigned int rsi_period{14};
};
#endif
