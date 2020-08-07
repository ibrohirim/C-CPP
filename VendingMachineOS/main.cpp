#include <iostream>
#include <string>
#include <vector>
#include "register.h"
#include "dispenser.h"

using namespace std;

void display(Dispenser *dispenser);
void operate(Register *money, Dispenser *dispenser);
//size of display area
const int width = 20;
const int height = 20;

int main(int argc, char *argv[]) {

    Register *money = new Register(0.0);

    Dispenser *dispenser = new Dispenser();

    display(dispenser);

    operate(money, dispenser);

    delete money, dispenser;

    return 0;
}

//recursive function to check money entered and ask for more or dispense and end

void operate(Register *money, Dispenser *dispenser) {

    vector<double> coins_in = money->money_entered();
    double total = money->get_total();
    cout << "Total money entered: $" << total << endl;
    string chosen = dispenser->get_user_selection();
    double purchase = dispenser->dispense(total, chosen);

    if(purchase != 0) {

        money->update_total(purchase);
        total = money->get_total();
        cout.precision(5);
        cout << "Amount left: $" << total << endl;
        cout << "Would you like another purchase? [y/n] ";
        string yn;
        cin >> yn;
        if(yn == "y") {

            display(dispenser);
            operate(money, dispenser);

        } else {

            ;

        }

    } else {

        operate(money, dispenser);

    }

}

//function to display products and pricing
void display(Dispenser *dispenser) {

    cout << "Machine accepts coins of 5, 10 and 25." << endl;
    vector<string> products = dispenser->to_vector();

    /*sitring products[] = {"A1:Milky Way $1.25", "A2:Snickers $1.00", "A3:Twix $1.00", "B1:Lays $0.75", "B2:Doritos $0.75", "B3:Ruffles $0.75", "C1:Turbo $0.25", "C2:Orbit $0.25", "C3:5-Gum $0.25" };*/
    int product_count = 0;
    //string array with products in the machine

    string space = "";
    unsigned sp_size = space.size();
    unsigned three_size = products[0].size();
    unsigned sp_new = three_size - sp_size;

    space.resize(sp_size+sp_new, ' ');

    string border = "";
    unsigned br_size = border.size();
    unsigned br_new = three_size - br_size;
    border.resize(br_size+br_new, '#');

    for(int i = 0; i < products.size(); i++) {

        if(products[i].length() < products[0].length()) {

            unsigned prd_i = products[i].size();
            unsigned prd_zero = products[0].size();

            unsigned new_size = prd_zero - prd_i;
            products[i].resize(prd_i+new_size, ' ');

        }

    }

    for(int i = 0; i < width; i++) {

        if(i == 0 || i == width-1){

            cout << "#";

        } else if(i%5 != 0) {

            cout << "#";

        } else {

            cout << border;

        }

    }
    cout << endl;

    for(int i = 0; i < height; i++) {

        for(int j = 0; j < width; j++) {

            if(j == 0 || j == width - 1) {

                cout << "#";

            } else if(j%5 == 0 && i % 5 == 0 && i != 0 && j != width) {

                if(product_count < products.size()) {
                    cout << products[product_count];
                    product_count += 1;
                }

            } else if(i%5 != 0 && j%5 == 0 && j != width && j%5 == 0) {

                cout << space;

            } else if(i == 0) {

                if(j%5 == 0 && j != width && j%5 == 0) {

                    cout << space;

                } else {

                    cout << " ";

                }

            } else {

                cout << " ";

            }

        }
        cout << endl;

    }


    for(int i = 0; i < width; i++) {

        if(i == 0 || i == width-1){

            cout << "#";

        } else if(i%5 != 0) {

            cout << "#";

        } else {

            cout << border;

        }

    }
    cout << endl;

    products.clear();
}
