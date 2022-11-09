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
    double principal;
    double curr_profit{0};
    int curr_position{0}; //declared int for pursose as we only want whole number
    vector<double> close_price;
    vector<double> daily_profit;

  public:
    enum class Signal{HOLD, BUY, SELL};
    
    Strategy(double prcpl) : principal(prcpl) {
    
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
    
    double calculate_rsi() {

    }
    
    //int division will truncate the the decimal places and round the number down
    void buy(int timeTick){
        if (curr_position == 0) {curr_position = principal / close_price[timeTick];}
        else if (curr_position < 0) {curr_position = 2 * principal / close_price[timeTick];}
        //it shud not be called if curr pos > 0
    }
    
    void sell(int timeTick){
        if (curr_position == 0) {curr_position = -principal / close_price[timeTick];}
        else if (curr_position > 0) {curr_position = -2 * principal / close_price[timeTick];}
        //it shud not be called if curr pos < 0
    }

    void stop(){
        curr_position = 0;
    }

    Signal rsi_signal(double rsi, double buySign, double sellSign, double holdSign){
        //current position is 0
        if (curr_position == 0){
            if (rsi > sellSign) {return Signal::SELL;}
            else if (rsi < buySign) {return Signal::BUY;}
            else {return Signal::HOLD;}
        }
        //current position is sell
        else if (curr_position < 0){
            if (buySign <= rsi <= holdSign) {return Signal::HOLD;}
            else if (rsi < buySign) {return Signal::BUY;}
            else {return Signal::SELL;}
        }
        //current position is buy can just use else instead
        else if (curr_position < 0){
            if (sellSign >= rsi >= holdSign) {return Signal::HOLD;}
            else if (rsi > sellSign) {return Signal::SELL;}
            else {return Signal::BUY;}
        }
    }

    void updatePortfolio(){

    }

    double sharpeRatio(){
        
    }


};



int main() {

  Strategy RSI(100000);
  RSI.read_data("BTC_data_daily.csv");
  RSI.print_close_price();

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