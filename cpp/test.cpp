#include "numb/numb.hpp"

void run(){
  Numb num1(1.28763);
  Numb num2(-23.5829);
  Numb num3 = num1 + num2;
  // cout << 1.28763 << " + " << -23.5829 << " = " << 1.28763 - 23.5829<< endl;
  cout << num1 << " + " << num2 << " = " << num3 << endl;
  Numb num4 = num1 + num2 + num3;
  cout << "sum [num1, num2, num3] = " << num4 << endl;
  Numb num5 = num4 * num1;
  Numb num6 = num5 / num4;
  Numb num7 = Numb::exp(num5);
}

int main(){
  cout << "+++ Generating cache1." << endl;
  Numb::setup("cache1", "", 15, "ignore-cache");
  run();
  cout << "+++ Generating cache2 with cache1 as witness." << endl;
  Numb::setup("cache2", "cache1", 15, "ignore-cache");
  run();
  cout << "+++ Loading cache1." << endl;
  Numb::setup("cache2", "cache1", 15, "load-cache");
  run();
  cout << "+++ Using cache1." << endl;
  Numb::setup("cache2", "cache1", 15, "use-cache");
  run();
}
