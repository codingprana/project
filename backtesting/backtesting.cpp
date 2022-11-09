#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <stdexcept>
#include <algorithm>

using namespace std;

class Strategy {
  private:
    double principal, buySign, sellSign, buyStopSign, sellStopSign;
    double curr_profit{0};
    int curr_position{0}; //declared int for pursose as we only want whole number
    unsigned int rsi_period{14};
    vector<double> close_price;
    vector<double> daily_profit;

  public:
    enum class Signal{HOLD, BUY, SELL};
    friend ostream& operator<<(ostream& os, const Signal &s);
    
    //ctor
    Strategy(double prcpl, double buy, double sell, double buyStop, double sellStop, unsigned int period) 
        : principal(prcpl)
        , buySign{buy}
        , sellSign{sell}
        , buyStopSign{buyStop}
        , sellStopSign(sellStop)
        , rsi_period{period} 
    {}

    void read_data(string file_name) {
      ifstream data(file_name, ios::in);
      if (!data) {
        cerr << "File could not be opened" << endl;
        exit(EXIT_FAILURE);
      }
      string str;
      getline (data, str);
      while (getline(data, str)) {
        istringstream iss(str);
        string token;
        getline(iss, token, ',');
        getline(iss, token, ',');
        getline(iss, token, ',');
        getline(iss, token, ',');
        getline(iss, token, ',');
        try {
          close_price.push_back(stod(token));
        }
        catch (invalid_argument) {
          cout << "Invalid argument";
        }
      }
    }

    void print_close_price() {
      unsigned int count{1};
      for (const double &i : close_price) {
        cout << count << ": " << i << endl;
        ++count;
      }
    }
    
    int get_data_size() const {
      return close_price.size();
    }

    double calculate_rsi(unsigned int timeTick = 0) {
      static double rsi{0}, ave_gain{0}, ave_loss{0};
      static int count{-1};

      if (timeTick == 0) { // initialize for signal function to iterate
        count = -1;
      }
      if (count == timeTick) { // call RSI without modifying
        return rsi;
      }
      else if (count + 1 == timeTick) { // make sure to iterate with time tick one by one
        // insufficient periods to calculate RSI
        if (timeTick < rsi_period) {
          // cout << timeTick << " is out of range ";
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

    }
    
    //int division will truncate the decimal places and round the number down
    void stop(int timeTick){
      int pos{curr_position}; // temp position
      curr_position = 0;
      principal -= (curr_position - pos)  * close_price[timeTick];
    }

    void buy(int timeTick){
      int pos{curr_position}; // temp position
      if (curr_position == 0) {curr_position = principal / close_price[timeTick];}
      else if (curr_position < 0) {stop(timeTick); curr_position = principal / close_price[timeTick];}
      //it shud not be called if curr pos > 0
      principal -= (curr_position - pos) * close_price[timeTick];
    }
    
    void sell(int timeTick){
      int pos{curr_position}; // temp position
      if (curr_position == 0) {curr_position = -1 * principal / close_price[timeTick];}
      else if (curr_position > 0) {stop(timeTick); curr_position = -1 * principal / close_price[timeTick];}
      //it shud not be called if curr pos < 0
      principal -= (curr_position - pos)  * close_price[timeTick];
    }


    Signal rsi_signal(double rsi){
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

    void update_portfolio(int timeTick){
      // double curr_principal = principal;
      double tempPortfolioValue{principal + curr_position * close_price[max(0, timeTick - 1)]};
      double curr_rsi = calculate_rsi(timeTick);
      Signal signal = rsi_signal(curr_rsi);

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
      cout << timeTick << "\t" << close_price[timeTick] << "\t" << curr_rsi << "\t" << signal << "\t"; 
      cout << principal << "\t" << curr_position << "\t" << daily_profit[timeTick] << "\t" << curr_profit << endl;
    }
};

ostream& operator<<(ostream& os, const Strategy::Signal &s){
  if (s == Strategy::Signal::BUY){os << "BUY";}
  else if (s == Strategy::Signal::SELL){os << "SELL";}
  else if (s == Strategy::Signal::HOLD){os << "HOLD";}
  else{os << "This should never happens..";}
  return os;
}



int main() {
  Strategy RSI(100000, 25, 75, 40, 60, 14);
  RSI.read_data("BTC_data_daily.csv");
  for (int i{0}; i < RSI.get_data_size(); ++i){
    RSI.update_portfolio(i);
  }
  

  // Strategy RSI2(1000000, 30, 70, 50, 14);
  // //RSI2.read_data("BTC_data_10-min.csv");
  // //RSI2.print_close_price();

  // Strategy RSI3(1000000, 30, 70, 50, 14);
  // //RSI3.read_data("BTC_data_5-min.csv");
  // //RSI3.print_close_price();

  // Strategy RSI4(1000000, 30, 70, 50, 14);
  // //RSI4.read_data("BTC_data_1-min.csv");
  // //RSI4.print_close_price();


  return 0;
}