#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>

using namespace std;

#ifndef REGISTER_H
#define REGISTER_H

class Register {
public:
    Register(double money);
    Register();

    vector<double> money_entered();
    double total_money(vector<double> money_in);
    void update_total(double price);
    double get_total();

private:
    vector<double>cents;
    double entered;
    double amount;
    double total;
    double all;
};

#endif