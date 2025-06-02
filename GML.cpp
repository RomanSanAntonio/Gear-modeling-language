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

map<string, Gear> gears;
vector<string> connectionOrder;
double inputRPM = 0;
double inputNM = 0;

double calcFrictionFunc(const string& func, double x) {
    if (func == "x") return x;
    if (func == "x^2") return x * x;
    if (func == "x^3") return x * x * x;
    if (func == "sqrt(x)") return sqrt(x);
    if (func == "log(x)") return log(x);
    if (func == "x/2") return x / 2;
    if (func == "x/3") return x / 3;
    return x;
}
