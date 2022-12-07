//RSI.cpp
#include <vector>
#include <iomanip>
#include "RSI.h"

using namespace std;

//constructor
RSI::RSI(string file_name, double prcpl, double buy, double sell, double buyStop, 
         double sellStop, unsigned int period) : Strategy(file_name, prcpl) {
    set_params(buy, sell, buyStop, sellStop, period);
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
  // double curr_principal = principal;
  double tempPortfolioValue{principal + curr_position * close_price[max(0, timeTick - 1)]};
  double curr_rsi = calculate_rsi(timeTick);
  Strategy::Signal signal = RSI::signal(curr_rsi);

  //update position
  if (curr_position == 0){
    if (signal == Signal::BUY){sell(timeTick);}
    else if (signal == Signal::SELL){buy(timeTick);}
  }
  else if (curr_position < 0){
    if (signal == Signal::BUY){sell(timeTick);}
    else if (signal == Signal::HOLD){stop(timeTick);}
  }
  else if (curr_position > 0){
    if (signal == Signal::SELL){buy(timeTick);}
    else if (signal == Signal::HOLD){stop(timeTick);}
  }

  double newPortfolioValue{principal + curr_position * close_price[timeTick]};
  daily_profit.push_back(newPortfolioValue - tempPortfolioValue);
  curr_profit += daily_profit[timeTick];
  
  if (show) display(timeTick, curr_rsi, signal);
}

void RSI::display(int timeTick, double curr_rsi, Signal signal) const{
  cout << setw(14) << timeTick << setw(14) << close_price[timeTick]
       << setw(14) << curr_rsi << setw(14) << signal << setw(14) << principal 
       << setw(14) << curr_position << setw(14) << daily_profit[timeTick] 
       << setw(14) << curr_profit << endl;
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

void RSI::set_params (double buy, double sell, double buyStop, 
                      double sellStop, unsigned int period) {
  rsi_period = period;
  buySign = buy;
  sellSign = sell;
  buyStopSign = buyStop;
  sellStopSign = sellStop;
  set_curr_profit(0);
  set_curr_position(0);
  daily_profit.clear();
}

vector<double> RSI::grid_search(double prcpl = 100000.0, 
                                double t = 365.0, double threshold = 2.0) {
  vector<vector<double>> v;

  for (int buy{20}; buy <= 40; ++buy) {
    for (int sell{60}; sell <= 80; ++sell) {
      for (int period{14}; period <= 21; ++period) {
        set_principal(prcpl);
        set_params(buy, sell, 50, 50, period);
        daily_profit.clear();
        for (int i{0}, end{this->get_data_size()}; i < end; ++i){
          this->update_portfolio(i, false);
        }
        this->set_sharpe_ratio(t); 
        // daily: 365, 10-min: 365*24*6, 5-min: 365*24*14, 1-min: 365*24*60

        if (this->get_sharpe_ratio() >= threshold) {
          v.push_back({curr_profit, this->get_sharpe_ratio(), 
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