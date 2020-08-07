#include <iostream>
#include <string>
#include <typeinfo>
#include "register.h"
#include <vector>

using namespace std;

Register::Register(double money) {
    total = money;
};

vector<double> Register::money_entered() {

    cents;
    entered;

    cout << "To submit amount entered enter 0." << endl;

    while(1) {

        cout << "Current total is: $" << total << endl;
        cout << "Enter amount of money: ";
        cin >> entered;

        if(!cin) {

            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input" << endl;

        }

        if(entered == 0) {
            break;
        }

        if(entered == 5 || entered == 10 || entered == 25 || entered == 50){

            entered = entered/100;
            this->total += entered;
            cents.push_back(entered);

        } else {
            cout << "Please enter coins of 5, 10, 25 and 50." << endl;
        }

    }

    return cents;

}

double Register::total_money(vector<double> money_in) {

    for(int i = 0; i < money_in.size(); i++) {

        all += money_in[i];

    }

    this->total = all;

    return all;
}

void Register::update_total(double price) {


    if(price != total) {

        this->total = total - price;
    } else {

        total = 0;

    }
}

double Register::get_total(){
    return this->total;
}