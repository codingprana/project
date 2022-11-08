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