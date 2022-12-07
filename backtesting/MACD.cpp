//MACD.cpp
#include <vector>
#include <iomanip>
#include "MACD.h"

using namespace std;

//constructor
MACD::MACD(string file_name, double prcpl = 100000.0,unsigned int fast = 12, 
           unsigned int slow = 26, unsigned int period = 9) : Strategy(file_name, prcpl) {
  reset_params(fast, slow, period);
}

void MACD::set_macd_fast(unsigned int fast){macd_fast = fast;}

void MACD::set_macd_slow(unsigned int slow){macd_slow = slow;}

void MACD::set_macd_period(unsigned int period){macd_period = period;}

void MACD::reset_params(unsigned int fast = 12, unsigned int slow = 26, unsigned int period = 9){
  //reset all params and indicators for new set of params
  set_macd_fast(fast);
  set_macd_slow(slow);
  set_macd_period(period);
  set_curr_position(0);
  set_curr_profit(0);
  daily_profit.clear();
  macd_line.clear();
  signal_line.clear();
}

unsigned int MACD::get_macd_fast(){return macd_fast;}

unsigned int MACD::get_macd_slow(){return macd_slow;}

unsigned int MACD::get_macd_period(){return macd_period;}

double MACD::calculate_moving_average(const vector<double>& vec, unsigned int timeTick, 
                                      unsigned int period) const{
  // Simple Moving Average
  // return -1 if not enough input
  // cout << vec[timeTick] << " " << timeTick << " " << period << endl;
  if((timeTick + 1) < period){return -1;}
  else{
    double sum{0.0};
    //maybe can use accumulate at <numeric>
    //need to use int i instead of unsigned int i as i can go below 0 at first case
    for (int i = static_cast<int>(timeTick); i >= (static_cast<int>(timeTick) + 1) - static_cast<int>(period); --i){
      sum += vec[i];
    }
    return (sum / period);
  }
}

void MACD::calculate_macd_line(unsigned int timeTick = 0){
  double fast_ma{calculate_moving_average(close_price, timeTick, macd_fast)};
  double slow_ma{calculate_moving_average(close_price, timeTick, macd_slow)};
  //Again =-1 if not enough input
  if(slow_ma == -1){macd_line.push_back(-1);}
  else{macd_line.push_back(fast_ma - slow_ma);}
}

void MACD::calculate_signal_line(unsigned int timeTick = 0){
  //this is essentially Moving average of MACD line
  //-1 if not enough input
  if ((timeTick + 2) < (macd_slow + macd_period)){signal_line.push_back(-1);}
  else{signal_line.push_back(calculate_moving_average(macd_line, timeTick, macd_period));}
}

MACD::Signal MACD::signal(double curr_macd, double curr_signal) const {
  // //Test strat 1
  // //This strat is simple, buy when both line above 0 and macd line > signal line, vice versa
  // if(curr_macd > 0 && curr_signal > 0 && curr_macd > curr_signal) {return Signal::BUY;}
  // else if(curr_macd < 0 && curr_signal < 0 && curr_macd < curr_signal) {return Signal::SELL;}
  // else{return Signal::HOLD;}

  //Test strat 2
  //This strat is simple, buy when macd line > signal line, vice versa freeing the >0 condition, this will trade more frequently
  //And we actually wont stop gain or loss in this strategy
  if(curr_macd > curr_signal) {return Signal::BUY;}
  else if(curr_macd < curr_signal) {return Signal::SELL;}
  else{return Signal::HOLD;}
}

void MACD::update_portfolio(int timeTick, bool show = false) {
  // double curr_principal = principal;
  double tempPortfolioValue{principal + curr_position * close_price[max(0, timeTick - 1)]};
  calculate_macd_line(timeTick);
  calculate_signal_line(timeTick);
  double curr_macd = macd_line[timeTick];
  double curr_signal = signal_line[timeTick];
  Strategy::Signal signal = MACD::signal(curr_macd, curr_signal);

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

  double newPortfolioValue{principal + curr_position * close_price[timeTick]};
  daily_profit.push_back(newPortfolioValue - tempPortfolioValue);
  curr_profit += daily_profit[timeTick];
  
  if (show) display(timeTick, curr_macd, curr_signal, signal);
}

void MACD::display(int timeTick, double curr_macd, double curr_signal, Signal signal) const{
  cout << setw(14) << timeTick << setw(14) << close_price[timeTick]
       << setw(14) << curr_macd << setw(14) << curr_signal << setw(14) << signal << setw(14) << principal 
       << setw(14) << curr_position << setw(14) << daily_profit[timeTick] 
       << setw(14) << curr_profit << endl;
}

void MACD::print_title(){
  cout << setw(14) << "Time" << setw(14) << "Close" << setw(14) << "MACD line"
       << setw(14) << "Signal line" << setw(14) << "Signal" << setw(14) << "Principal"
       << setw(14) << "Position" << setw(14) << "Curr. Profit"
       << setw(14) << "Total Profit" << endl;
}

void MACD::calculate_portfolio(bool show = false){
  if (show) print_title();
  for(unsigned long timeTick{0}; timeTick < close_price.size(); ++timeTick){
    update_portfolio(timeTick, show);
  }
}

vector<double> MACD::grid_search(unsigned int fast_low, unsigned int fast_high, 
                                 unsigned int slow_low, unsigned int slow_high,
                                 unsigned int period_low, unsigned int period_high, 
                                 double prcpl = 100000.0, 
                                 double t = 365.0, double threshold = 2.0) {
  vector<vector<double>> v;
  for(unsigned int fast{fast_low}; fast <= fast_high; ++fast){
    for(unsigned int slow{slow_low}; slow <= slow_high; ++slow){
      for(unsigned int period{period_low}; period <= period_high; ++period){
        set_principal(prcpl);
        reset_params(fast, slow, period);
        calculate_portfolio(false);
        set_sharpe_ratio(t); 
        if (this->get_sharpe_ratio() >= threshold) {
          v.push_back({curr_profit, this->get_sharpe_ratio(), 
                       static_cast<double>(fast), 
                       static_cast<double>(slow), 
                       static_cast<double>(period), 
                      });
        }
      }
    }
  }
  
  sort(v.begin(), v.end(), greater());
  return v[0];
}