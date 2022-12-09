//RSI.h
//RSI class derived from class
//Strategy

#ifndef RSI_H
#define RSI_H

#include <vector>
#include "Strategy.h"

class RSI : public Strategy {
public:
  // ctor & dtor
  RSI();
  RSI(double);
  RSI(std::string);
  RSI(std::string, double, double, double, int,
      double, double, double, double, unsigned int);
  virtual ~RSI() = default;

  // getters
  double get_buySign() const;
  double get_sellSign() const;
  double get_buyStopSign() const;
  double get_sellStopSign() const;
  unsigned int get_rsi_period() const;

  // setters
  void set_buySign(double);
  void set_sellSign(double);
  void set_buyStopSign(double);
  void set_sellStopSign(double);
  void set_rsi_period(unsigned int);
  void set_RSI_params(double, double, double, double, unsigned int);

  // for RSI strategy trading
  double calculate_rsi(unsigned int) const;
  Signal signal(double) const; //get trading signal
  virtual void update_portfolio(int, bool) override;
  virtual void calculate_portfolio(bool) override;

  // output
  virtual void print_title() override;
  void display(int, double, Signal) const;

  // for finding optimal strategy
  virtual std::vector<double> grid_search(double, double, double);

private:
  double buySign{0}, sellSign{0}, buyStopSign{0}, sellStopSign{0};
  unsigned int rsi_period{14};
};
#endif
