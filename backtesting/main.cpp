#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <stdexcept>

using namespace std;

class Strategy {
  private:
    double principle;
    double curr_profit{0};
    int curr_position{0};
    unsigned int rsi_period{14};
    vector<double> close_price;
    vector<double> daily_profit;

  public:
    Strategy(double prcpl) : principle(prcpl) {
    
    }

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

      if (timeTick < rsi_period) {
        cout << timeTick << " is out of range ";
        return -1;
      }

      // first RSI
      if (timeTick == rsi_period) {
        for (unsigned int i{0}; i < rsi_period; ++i) {
          double dif{close_price[i+1] - close_price[i]};
          ave_gain += ( dif > 0 ? dif : 0);
          ave_loss += ( dif < 0 ? -dif : 0);
        }
        ave_gain /= rsi_period;
        ave_loss /= rsi_period;
        rsi = 100.0 - 100.0 / (1 + ave_gain / ave_loss);
        return rsi;
      }

      // RSI other than first
      double dif{close_price[timeTick] - close_price[timeTick-1]};
      ave_gain = (ave_gain * (rsi_period - 1) + ( dif > 0 ? dif : 0) ) / rsi_period;
      ave_loss = (ave_loss * (rsi_period - 1) + ( dif < 0 ? -dif : 0) ) / rsi_period;
      rsi = 100.0 - 100.0 / (1 + ave_gain / ave_loss);

      return rsi;
    }


};



int main() {

  Strategy RSI(100000);
  RSI.read_data("BTC_data_daily.csv");
  RSI.print_close_price();
  cout << endl;
  for (int i{0}; i < RSI.get_data_size(); ++i) {
    cout << i << ": " << RSI.calculate_rsi(i) << endl;
  }

  Strategy RSI2(100000);
  //RSI2.read_data("BTC_data_10-min.csv");
  //RSI2.print_close_price();

  Strategy RSI3(100000);
  //RSI3.read_data("BTC_data_5-min.csv");
  //RSI3.print_close_price();

  Strategy RSI4(100000);
  //RSI4.read_data("BTC_data_1-min.csv");
  //RSI4.print_close_price();


  return 0;
}