//RSI.cpp
#include <vector>
#include <iomanip>
#include "RSI.h"

using namespace std;

// ctor
RSI::RSI() : Strategy() {
  set_RSI_params(0, 0, 0, 0, 14);
}

RSI::RSI(double prcpl) : Strategy(prcpl) {
  set_RSI_params(0, 0, 0, 0, 14);
}

RSI::RSI(string file_name) : Strategy(file_name) {
  set_RSI_params(0, 0, 0, 0, 14);
}

RSI::RSI(string file_name, double sharp_r, double prcpl, double profit, int pos, 
         double buy, double sell, double buyStop, double sellStop, unsigned int period)
         // explicitly call base-class ctor
         : Strategy(file_name, sharp_r, prcpl, profit, pos) {
    set_RSI_params(buy, sell, buyStop, sellStop, period);
}


// getters
double RSI::get_buySign() const {
  return buySign;
}

double RSI::get_sellSign() const {
  return sellSign;
}

double RSI::get_buyStopSign() const {
  return buyStopSign;
}

double RSI::get_sellStopSign() const {
  return sellStopSign;
}

unsigned int RSI::get_rsi_period() const {
  return rsi_period;
}


// setters
void RSI::set_buySign(double signal) {
  buySign = signal;
}

void RSI::set_sellSign(double signal) {
  sellSign = signal;
}

void RSI::set_buyStopSign(double signal) {
  buyStopSign = signal;
}

void RSI::set_sellStopSign(double signal) {
  sellStopSign = signal;
}

void RSI::set_rsi_period(unsigned int period) {
  rsi_period = period;
}

void RSI::set_RSI_params (double buy, double sell, double buyStop, 
                      double sellStop, unsigned int period) {
  // set parameters for RSI strategy
  set_buySign(buy);
  set_sellSign(sell);
  set_buyStopSign(buyStop);
  set_sellStopSign(sellStop);
  set_rsi_period(period);
}

double RSI::calculate_rsi(unsigned int timeTick = 0) const {
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

RSI::Signal RSI::signal(double rsi) const {
  int curr_position = get_curr_position();
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

void RSI::update_portfolio(int timeTick, bool show = false) {
  double tempPortfolioValue{get_principal() + get_curr_position() * close_price[max(0, timeTick - 1)]};
  double curr_rsi = calculate_rsi(timeTick);
  Strategy::Signal signal = RSI::signal(curr_rsi);

  int curr_position = get_curr_position();
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

  double newPortfolioValue{get_principal() + get_curr_position() * close_price[timeTick]};
  period_profit.push_back(newPortfolioValue - tempPortfolioValue);
  set_curr_profit(get_curr_profit() + period_profit[timeTick]);
  
  if (show) display(timeTick, curr_rsi, signal);
}

void RSI::display(int timeTick, double curr_rsi, Signal signal) const{
  cout << setw(14) << timeTick << setw(14) << close_price[timeTick]
       << setw(14) << curr_rsi << setw(14) << signal << setw(14) << get_principal()
       << setw(14) << get_curr_position() << setw(14) << period_profit[timeTick] 
       << setw(14) << get_curr_profit() << endl;
}

void RSI::print_title(){
  cout << setw(14) << "Time" << setw(14) << "Close" << setw(14) << "RSI"
       << setw(14) << "Signal" << setw(14) << "Principal"
       << setw(14) << "Position" << setw(14) << "Curr. Profit"
       << setw(14) << "Total Profit" << endl;
}

void RSI::calculate_portfolio(bool show = false){
  if (show) print_title();
  for(unsigned long timeTick{0}; timeTick < close_price.size(); ++timeTick){
    update_portfolio(timeTick, show);
  }
}

// return the optimal parameters for backtesting
vector<double> RSI::grid_search(double prcpl = 100000.0, 
                                double t = 365.0, double threshold = 2.0) {
  vector<vector<double>> v;

  int end = get_data_size();
  // buy grid search range: 20 ~ 40 
  for (int buy{20}; buy <= 40; ++buy) {
    // sell grid search range: 60 ~ 80
    for (int sell{60}; sell <= 80; ++sell) {
      // period grid search range: 14 ~ 21
      for (int period{14}; period <= 21; ++period) {
        reset_strategy(prcpl);
        set_RSI_params(buy, sell, 50, 50, period);
        for (int i{0}; i < end; ++i){
          update_portfolio(i, false);
        }
        calculate_sharpe_ratio(t); 

        if (get_sharpe_ratio() >= threshold) {
          v.push_back({get_curr_profit(), get_sharpe_ratio(), 
                       static_cast<double>(period), 
                       static_cast<double>(buy), 
                       static_cast<double>(sell), 
                      });
        }
      }
    }
  }
  sort(v.begin(), v.end(), greater());
  return v[0];
}