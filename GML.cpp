#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <functional>

using namespace std;

struct Gear {
    string name;
    int teeth = 0;
    double RPM = 0.0;
    double NM = 0.0;
    double heat = 20.0;
    function<double(double)> friction = nullptr;
    double resistance = 0.0;
    string id = "";
    string spcl = "";

    vector<string> connected;
    bool isOutput = false;
    bool visited = false;
};

map<string, Gear> gears;
double inputRPM = 0.0;
double inputNM = 0.0;

// === Friction function parser ===
function<double(double)> getFrictionFunc(string expr) {
    if (expr == "x^2") {
        return [](double x) { return x * x; };
    } else if (expr == "x") {
        return [](double x) { return x; };
    } else if (expr == "sqrt(x)") {
        return [](double x) { return sqrt(x); };
    } else {
        return nullptr;
    }
}

// === Parse a line like: g1 = 8[h=20, f=x^2, r=5, id="main", spcl={...}] ===
void parseGear(string line) {
    stringstream ss(line);
    string name, equals, rest;
    ss >> name >> equals;
    getline(ss, rest);

    Gear g;
    g.name = name;

    size_t bracketPos = rest.find("[");
    if (bracketPos != string::npos) {
        g.teeth = stoi(rest.substr(0, bracketPos));
        string attributes = rest.substr(bracketPos + 1, rest.find("]") - bracketPos - 1);
        stringstream attrss(attributes);
        string token;
        while (getline(attrss, token, ',')) {
            size_t eq = token.find('=');
            if (eq == string::npos) continue;
            string key = token.substr(0, eq);
            string val = token.substr(eq + 1);
            key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
            val.erase(remove_if(val.begin(), val.end(), ::isspace), val.end());

            if (key == "h") g.heat = stod(val);
            else if (key == "f") g.friction = getFrictionFunc(val);
            else if (key == "r") g.resistance = stod(val);
            else if (key == "id") g.id = val;
            else if (key == "spcl") g.spcl = val;
        }
    } else {
        g.teeth = stoi(rest);
    }

    gears[name] = g;
}

// === Parse underscore connection format ===
void parseConnection(string line) {
    stringstream ss(line);
    string token;
    vector<string> tokens;
    while (getline(ss, token, '_')) {
        if (!token.empty()) tokens.push_back(token);
    }

    for (size_t i = 0; i < tokens.size(); ++i) {
        string tok = tokens[i];
        if (tok == "s") continue;
        else if (tok == "o") {
            if (i + 1 < tokens.size()) gears[tokens[i + 1]].isOutput = true;
        }
        else if (tok == "c" || tok == "i") {
            if (i > 0 && i + 1 < tokens.size()) {
                string a = tokens[i - 1], b = tokens[i + 1];
                gears[a].connected.push_back(b);
                gears[b].connected.push_back(a);
            }
        }
    }
}

// === Recursive RPM/NM distribution ===
void distribute(string name, double rpm, double nm) {
    Gear& g = gears[name];
    if (g.visited) return;
    g.visited = true;
    g.RPM = rpm;
    g.NM = nm;

    double loss = 0;
    if (g.friction) {
        double fval = g.friction(rpm);
        if (fval >= nm) {
            g.RPM = 0;
            loss = nm;
        } else {
            g.NM = nm - fval;
            loss = fval;
        }
    }
    g.heat += (rpm / 2.0) + (loss / 10.0);

    for (string& conn : g.connected) {
        Gear& next = gears[conn];
        if (!next.visited) {
            double ratio = (double)g.teeth / (double)next.teeth;
            double nextRPM = rpm / ratio;
            double nextNM = g.NM * ratio;
            distribute(conn, nextRPM, nextNM);
        }
    }
}

void applyInput(double rpm, double nm) {
    for (auto& [name, g] : gears) {
        if (!g.visited) {
            distribute(name, rpm, nm);
            break;
        }
    }
}

void printOutputs() {
    for (auto& [name, g] : gears) {
        if (g.isOutput) {
            cout << name << ":\n";
            cout << "  RPM: " << g.RPM << "\n";
            cout << "  NM: " << g.NM << "\n";
            cout << "  Heat: " << g.heat << "\n";
        }
    }
}

// === MAIN ENTRY ===
int main() {
    cout << "Paste your GML code below. Press Ctrl+D (or Ctrl+Z then Enter on Windows) to run:\n";

    stringstream inputBuffer;
    string line;
    while (getline(cin, line)) {
        inputBuffer << line << "\n";
    }

    string inputText = inputBuffer.str();
    stringstream ss(inputText);

    while (getline(ss, line)) {
        if (line.find('=') != string::npos && line.find("_") == string::npos) {
            parseGear(line);
        } else if (line.find("RPM:") != string::npos) {
            inputRPM = stod(line.substr(line.find(":") + 1));
        } else if (line.find("NM:") != string::npos) {
            inputNM = stod(line.substr(line.find(":") + 1));
        } else if (line.find("_") != string::npos) {
            parseConnection(line);
        }
    }

    applyInput(inputRPM, inputNM);
    printOutputs();

    return 0;
}
