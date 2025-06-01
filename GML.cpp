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
  int teeth;
  double heat;
  string fricfunc = "";
  string id = "";
  double rpm;
  double nm;
  double resistance;
};
