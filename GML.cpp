#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <regex>
#include <cmath>

using namespace std;

struct Gear {
  string name;
  int teeth = 1;
  double heat = 20;
  string fricfunc = "x^2";
  string id = "";
  double rpm = 0;
  double nm = 0;
  bool isoutput = false;
  double resistance = 0;
};
