#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <climits>

using namespace std;

bool isPrime(const size_t);
void displayPrimes(size_t, size_t);
bool fastPrimeTest(const size_t);
void updatePrimes(size_t);
void printPrimes(size_t);

vector<size_t> primes{2, 3};

int main() {
  clock_t timer;
  size_t num{2}, item{1};
  char choice;

  for (;;) {
    do {
      cout << "   1. Test Function: isPrime" << endl;
      cout << "   2. Test Function: displayPrimes" << endl;
      cout << "   3. Test Function: printPrimes" << endl;
      cout << "   Choose One (q to quit): ";
      cin >> choice;
      cout << "\n\n";
    } while (choice < '1' || choice > '3' && choice != 'q');

    if (choice == 'q') break;
    
    switch(choice) {
      case '1':
        cout << "isPrime(n): return '1' if the number 'n' is a prime, '0' otherwise \n\n";
        for(;;) {
          cout << "Give One Positive Integer (1 < n < 2^64-1): ";
          cin >> num;
          if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
          }
          break;
        }
        timer = clock();
        cout << "isPrime(" << num << "): " << isPrime(num);
        break;

      case '2':
        cout << "displayPrimes(n, k): ";
        cout << "print out the last 'k' prime numbers ";
        cout << "close to but not larger than the number 'n' \n\n";
        for(;;) {
          cout << "Give Positive Integer n (1 < n < 2^64): ";
          cin >> num;
          if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
          }
          break;
        }
        for(;;) {
          cout << "Give Positive Integer k (1 < k < 2^64): ";
          cin >> item;
          if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
          }
          break;
        }
        timer = clock();
        cout << "displayPrimes(" << num << ", " << item << "): ";
        displayPrimes(num, item);
        break;
      case '3':
        cout << "printPrimes(n): ";
        cout << "print out all primes under a given number 'n' \n\n";
        for(;;) {
          cout << "Give Positive Integer n (1 < n < 2^64): ";
          cin >> num;
          if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
          }
          break;
        }
        timer = clock();
        cout << endl;
        printPrimes(num);
        break;

      default:
        break;
    }
    cout << "\n\n";
    timer = clock() - timer;
    cout << "Run Time: " << (float)timer/CLOCKS_PER_SEC << " sec \n\n\n";
  }
  return 0;
}

bool isPrime(const size_t num) {
  //return '1' if the number 'n' is a prime, '0' otherwise
  if (2 == num || 3 == num) return true;
  if ( !(num % 2) || !(num % 3) || num < 2 ) return false;

  for (size_t i{5}; UINT_MAX >= i && num >= i * i; i += 6) {
    if ( num % i == 0 || num % (i+2) == 0) {
      return false; 
    }
  }
  return true;
}

void displayPrimes(size_t num, size_t k) {
  //print out the last 'k' prime numbers below the number 'num'
  cout << endl;
  if (num < 2) return;
  if (num % 2 == 0) --num;
  for (size_t n{num}, count{1}; n >= 3; n -= 2) {
    if (!k) return;
    if (isPrime(n)) {
      cout << n << "\t"; 
      --k;
      ++count;
      if (count >= 6) {
        cout << endl;
        count = 1;
      }  
    }
  }
  if (k) cout << 2;
}

bool fastPrimeTest(const size_t num) {
  const unsigned int size{ static_cast<unsigned int>(::primes.size()) };
  for (unsigned int i{1}; num >= ::primes[i] * ::primes[i] && size > i; ++i) {
    if ( num % ::primes[i] == 0) return false;
  }
  return true;
}

void updatePrimes(size_t num) {
  //update the list of primes
  const size_t lastPrime{ ::primes[::primes.size()-1] };
  for (size_t n{lastPrime + 2}; n <= num; n += 2) {
    const unsigned int test{ static_cast<unsigned int>(sqrt(static_cast<double>(n))) };
    for (unsigned int i{1}; ::primes[i] <= test; ++i) {
      if (n % ::primes[i] == 0) goto endloop;
    }
    ::primes.push_back(n);
    endloop: ;
  }
}

void printPrimes(size_t num) {
  updatePrimes(num);
  unsigned int count{1};
  for (const unsigned int &prime: ::primes) {
    if (num < prime) break;
    cout << "(" << count << ") " << prime << "\t";
    if ( count % 10 == 0 ) cout << endl;
    ++count;
  }
}

/*

Test Flag 2: displayPrimes
displayPrimes(n, k): print out the last 'k' prime numbers close to but not larger than the number 'n' 

Give Positive Integer n (1 < n < 2^64): 18446744073709551615
Give Positive Integer k (1 < k < 2^64): 1
displayPrimes(18446744073709551615, 1): 18446744073709551557 

Run Time: 11.5431 sec 


New Record:


   1. Test Function: isPrime
   2. Test Function: displayPrimes
   3. Test Function: printPrimes
   Choose One (q to quit): 2


displayPrimes(n, k): print out the last 'k' prime numbers close to but not larger than the number 'n' 

Give Positive Integer n (1 < n < 2^64): 9999999999999999
Give Positive Integer k (1 < k < 2^64): 10
displayPrimes(9999999999999999, 10): 
9999999999999937 9999999999999917 9999999999999887 9999999999999851 9999999999999817 
9999999999999809 9999999999999671 9999999999999643 9999999999999641 9999999999999631 

Run Time: 3.37867 sec 


   1. Test Function: isPrime
   2. Test Function: displayPrimes
   3. Test Function: printPrimes
   Choose One (q to quit): 2


displayPrimes(n, k): print out the last 'k' prime numbers close to but not larger than the number 'n' 

Give Positive Integer n (1 < n < 2^64): 99999999999999999
Give Positive Integer k (1 < k < 2^64): 10
displayPrimes(99999999999999999, 10): 
99999999999999997 99999999999999977 99999999999999961 99999999999999943 99999999999999919 
99999999999999823 99999999999999761 99999999999999739 99999999999999727 99999999999999587 

Run Time: 8.99028 sec 


   1. Test Function: isPrime
   2. Test Function: displayPrimes
   3. Test Function: printPrimes
   Choose One (q to quit): 2


displayPrimes(n, k): print out the last 'k' prime numbers close to but not larger than the number 'n' 

Give Positive Integer n (1 < n < 2^64): 999999999999999999
Give Positive Integer k (1 < k < 2^64): 10
displayPrimes(999999999999999999, 10): 
999999999999999989 999999999999999967 999999999999999877 999999999999999863 999999999999999829 
999999999999999749 999999999999999737 999999999999999709 999999999999999637 999999999999999631 

Run Time: 28.1434 sec 


   1. Test Function: isPrime
   2. Test Function: displayPrimes
   3. Test Function: printPrimes
   Choose One (q to quit): 2                  


displayPrimes(n, k): print out the last 'k' prime numbers close to but not larger than the number 'n' 

Give Positive Integer n (1 < n < 2^64): 9999999999999999999
Give Positive Integer k (1 < k < 2^64): 10
displayPrimes(9999999999999999999, 10): 
9999999999999999961 9999999999999999943 9999999999999999919 9999999999999999877 9999999999999999817 
9999999999999999751 9999999999999999719 9999999999999999701 9999999999999999679 9999999999999999673 

Run Time: 86.7514 sec 


   1. Test Function: isPrime
   2. Test Function: displayPrimes
   3. Test Function: printPrimes
   Choose One (q to quit): 2


displayPrimes(n, k): print out the last 'k' prime numbers close to but not larger than the number 'n' 

Give Positive Integer n (1 < n < 2^64): 18446744073709551615
Give Positive Integer k (1 < k < 2^64): 10
displayPrimes(18446744073709551615, 10): 
18446744073709551557 18446744073709551533 18446744073709551521 18446744073709551437 18446744073709551427 
18446744073709551359 18446744073709551337 18446744073709551293 18446744073709551263 18446744073709551253 

Run Time: 126.238 sec 

--------------------------------------------------------------------------------------------------------
Old Record:


   1. Test Tuncfion: isPrime
   2. Test Tuncfion: displayPrimes
   Choose One (q to quit): 2


Test Flag 2: displayPrimes(vector<unsigned size_t>& primes, unsigned size_t n, unsigned size_t k) 
print out the last 'k' prime numbers below the number 'n' 

Give One Positive Integer (1 <= n <= 2^64-1): 9999999999999999
Give One Positive Integer (1 <= k <= 2^64-1): 10
displayPrimes(9999999999999999, 10) => 
9999999999999937 9999999999999917 9999999999999887 9999999999999851 9999999999999817 
9999999999999809 9999999999999671 9999999999999643 9999999999999641 9999999999999631 

Run Time: 45.787 sec 


   1. Test Tuncfion: isPrime
   2. Test Tuncfion: displayPrimes
   Choose One (q to quit): 2


Test Flag 2: displayPrimes(vector<unsigned size_t>& primes, unsigned size_t n, unsigned size_t k) 
print out the last 'k' items of primes below the number 'n' 

Give One Positive Integer (1 <= n <= 2^64-1): 99999999999999999
Give One Positive Integer (1 <= k <= 2^64-1): 10
displayPrimes(99999999999999999, 10) => 
99999999999999997 99999999999999977 99999999999999961 99999999999999943 99999999999999919 
99999999999999823 99999999999999761 99999999999999739 99999999999999727 99999999999999587 

Run Time: 165.862 sec 


   1. Test Tuncfion: isPrime
   2. Test Tuncfion: displayPrimes
   Choose One (q to quit): 2


Test Flag 2: displayPrimes(vector<unsigned size_t>& primes, unsigned size_t n, unsigned size_t k) 
print out the last 'k' items of primes below the number 'n' 

Give One Positive Integer (1 <= n <= 2^64-1): 999999999999999999
Give One Positive Integer (1 <= k <= 2^64-1): 10
displayPrimes(999999999999999999, 10) => 
999999999999999989 999999999999999967 999999999999999877 999999999999999863 999999999999999829 
999999999999999749 999999999999999737 999999999999999709 999999999999999637 999999999999999631 

Run Time: 868.06 sec 


   1. Test Tuncfion: isPrime
   2. Test Tuncfion: displayPrimes
   Choose One (q to quit): 2


Test Flag 2: displayPrimes(vector<unsigned size_t>& primes, unsigned size_t n, unsigned size_t k) 
print out the last 'k' items of primes below 'n' 

Give One Positive Integer (1 <= n <= 2^64-1): 18446744073709551615
Give One Positive Integer (1 <= k <= 2^64-1): 10
displayPrimes(18446744073709551615, 10) => 
18446744073709551557 18446744073709551533 18446744073709551521 18446744073709551437 18446744073709551427 
18446744073709551359 18446744073709551337 18446744073709551293 18446744073709551263 18446744073709551253 

Run Time: 10894 sec 

*/