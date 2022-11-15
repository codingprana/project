// Strategy.h

#include <vector>
#include <iostream>
#include <string>

#ifndef STRATEGY_H
#define STRATEGY_H

class Strategy {
  private:
    double principal{0}, buySign{0}, sellSign{0}, buyStopSign{0}, sellStopSign{0};
    double curr_profit{0};
    int curr_position{0}; //declared int for pursose as we only want whole number
    unsigned int rsi_period{14};
    std::vector<double> close_price;
    std::vector<double> daily_profit;

  public:
    enum class Signal{HOLD, BUY, SELL};
    //friend std::ostream& operator<<(std::ostream&, const Signal);
    
    //constructor
    Strategy(double, double, double, double, double, unsigned int);

    void read_data(std::string);
    void print_close_price() const;
    int get_data_size() const;
    double calculate_rsi(unsigned int) const;

    //int division will truncate the decimal places and round the number down
    void stop(int);
    void buy(int);
    void sell(int);
    Signal rsi_signal(double) const;
    void update_portfolio(int);
};

#endif