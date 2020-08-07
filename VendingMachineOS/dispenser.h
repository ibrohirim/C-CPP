#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

#ifndef DISPENSER_H
#define DISPENSER_H

class Dispenser {
private:
    string choice;
    map<string, string> items;
    map<string, double> price;
    map<string, int> quantity;

public:
    Dispenser();
    ~Dispenser();
    string get_user_selection();
    double dispense(double money, string code);
    vector<string> to_vector();
};

#endif