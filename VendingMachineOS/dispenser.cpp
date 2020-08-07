#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "dispenser.h"

using namespace std;

Dispenser::Dispenser() {

    items["A1"] = "Milky Way";
    items["A2"] = "Snickers";
    items["A3"] = "Twix";
    items["B1"] = "Lays";
    items["B2"] = "Doritos";
    items["B3"] = "Ruffles";
    items["C1"] = "Turbo";
    items["C2"] = "Orbit";
    items["C3"] = "5-Gum";


    price["A1"] = 1.25;
    price["A2"] = 1.00;
    price["A3"] = 1.00;
    price["B1"] = 0.75;
    price["B2"] = 0.75;
    price["B3"] = 0.75;
    price["C1"] = 0.25;
    price["C2"] = 0.25;
    price["C3"] = 0.25;

    quantity["A1"] = 1;
    quantity["A2"] = 25;
    quantity["A3"] = 25;
    quantity["B1"] = 25;
    quantity["B2"] = 25;
    quantity["B3"] = 25;
    quantity["C1"] = 25;
    quantity["C2"] = 25;
    quantity["C3"] = 25;
}

/*Dispenser::~Dispenser() {

    delete &items, &price, &quantity, &choice;

}*/

string Dispenser::get_user_selection(){

    string choice;

    cout << "Please select the treat you desire using the code: ";
    cin >> choice;
    return choice;
}

double Dispenser::dispense(double money, string code) {

    if(price.find(code) == price.end()) {

        cout << "Invalid code, please try again" << endl;
        return 0;

    } else {


        if(quantity[code] > 0) {

            if(money >= price[code]) {

                cout << "Here's your item: " << items[code] << endl;
                quantity[code] = quantity[code] - 1;
                return price[code];

            } else if(money < price[code]) {

                cout.precision(5);
                cout << "Please add another $" << price[code] - money << " to buy " << items[code] << endl;

                return 0;

            } else {

                return 0;

            }
        } else {

            cout << "Sorry we are out of: " << items[code] << " Please make another selection."<< endl;

        }
    }
}

vector<string> Dispenser::to_vector() {

    vector<string> array;

    for(map<string, string>::iterator it=items.begin(); it !=items.end(); ++it) {

        string to_push = "";
        to_push.append(it->first);
        to_push.append(": ");
        to_push.append(it->second);
        array.push_back(to_push);

    }

    int counter = 0;

    for(map<string, double>::iterator it=price.begin(); it != price.end(); ++it) {

        ostringstream oss;
        oss << it->second;
        array[counter].append(" $");
        array[counter].append(oss.str());
        counter += 1;
    }

    counter = 0;

    for(map<string, int>::iterator it=quantity.begin(); it != quantity.end(); ++it) {

        ostringstream oss;
        oss << it->second;
        array[counter].append(" Qty: ");
        array[counter].append(oss.str());
        counter += 1;
    }

    return array;

}