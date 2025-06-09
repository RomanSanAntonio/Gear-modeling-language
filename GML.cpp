#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <regex>

using namespace std;

struct Gear {
    string name;
    double teeth;
    double RPM = 0.0;
    double NM = 0.0;
    vector<string> connected;
};

unordered_map<string, Gear> gears;
unordered_map<string, double> rpmInputs;
unordered_map<string, double> nmInputs;

// Trim spaces
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Parse gear assignments like "g1 = 8"
void parseGear(const string& line) {
    regex r(R"((\w+)\s*=\s*([0-9.]+))");
    smatch m;
    if (regex_match(line, m, r)) {
        Gear g;
        g.name = m[1];
        g.teeth = stod(m[2]);
        gears[g.name] = g;
    }
}

// Parse connections like g1_c_g2_i_g3
void parseConnection(const string& line) {
    vector<string> tokens;
    string temp;
    for (char c : line) {
        if (c == '_') {
            if (!temp.empty()) {
                tokens.push_back(temp);
                temp.clear();
            }
        } else {
            temp += c;
        }
    }
    if (!temp.empty()) tokens.push_back(temp);

    for (size_t i = 0; i < tokens.size() - 1; i++) {
        string g1 = tokens[i];
        string g2 = tokens[i + 1];

        // If it's an operation like "c", skip
        if (g1 == "c" || g1 == "i" || g1 == "o" || g1 == "s") continue;
        if (g2 == "c" || g2 == "i" || g2 == "o" || g2 == "s") continue;

        gears[g1].connected.push_back(g2);
    }
}

// Parse input lines like RPM:g1=40,g3=10
void parseInputLine(const string& line, bool isRPM) {
    string data = line.substr(line.find(":") + 1);
    stringstream ss(data);
    string item;
    while (getline(ss, item, ',')) {
        size_t eq = item.find('=');
        if (eq != string::npos) {
            string name = trim(item.substr(0, eq));
            double val = stod(trim(item.substr(eq + 1)));
            if (isRPM) rpmInputs[name] = val;
            else nmInputs[name] = val;
        }
    }
}

void applyInputs() {
    for (const auto& [name, rpm] : rpmInputs) {
        if (gears.count(name)) gears[name].RPM = rpm;
    }
    for (const auto& [name, nm] : nmInputs) {
        if (gears.count(name)) gears[name].NM = nm;
    }
}

void simulate() {
    applyInputs();

    cout << "\n--- Output ---" << endl;
    for (auto& [name, gear] : gears) {
        if (gear.RPM != 0 || gear.NM != 0) {
            cout << name << ":\n";
            cout << "  RPM: " << gear.RPM << "\n";
            cout << "  NM : " << gear.NM << "\n";
        }
    }
}

int main() {
    cout << "Enter your GML code below (type END to finish):\n";

    string line;
    while (getline(cin, line)) {
        line = trim(line);
        if (line == "END") break;

        if (line.find('=') != string::npos && line.find("_") == string::npos && line.find(':') == string::npos) {
            parseGear(line);
        } else if (line.find("RPM:") == 0) {
            parseInputLine(line, true);
        } else if (line.find("NM:") == 0) {
            parseInputLine(line, false);
        } else {
            parseConnection(line);
        }
    }

    simulate();
    return 0;
}
