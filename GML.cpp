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


void parseGear(string line) {
    regex rgx(R"((\w+)\s*=\s*(\d+)(?:\[(.*?)\])?)");
    smatch match;
    if (regex_match(line, match, rgx)) {
        Gear g;
        g.name = match[1];
        g.teeth = stod(match[2]);
        if (match[3].matched) {
            string attr = match[3];
            regex propRgx(R"((\w+)\s*=\s*([^,\]]+))");
            auto begin = sregex_iterator(attr.begin(), attr.end(), propRgx);
            auto end = sregex_iterator();
            for (auto i = begin; i != end; ++i) {
                string key = (*i)[1];
                string val = (*i)[2];
                if (key == "f") g.frictionFunc = val;
                else if (key == "h") g.heat = stod(val);
                else if (key == "r") g.resistance = stod(val);
            }
        }
        gears[g.name] = g;
    }
}

void parseConnection(string line) {
    stringstream ss(line);
    string token;
    while (getline(ss, token, '_')) {
        if (!token.empty()) connectionOrder.push_back(token);
    }
}

void applyInput(double rpm, double nm) {
    inputRPM = rpm;
    inputNM = nm;
    for (size_t i = 0; i < connectionOrder.size(); ++i) {
        string current = connectionOrder[i];
        if (current == "s") {
            string nextGear = connectionOrder[i + 1];
            gears[nextGear].rpm = rpm;
            gears[nextGear].nm = nm;
        }
    }
}
