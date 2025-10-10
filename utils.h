/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 9/23/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp -o serendipity.out
*/

#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <iomanip>

using namespace std;

void clearScreen();

void pressEnterToContinue();

struct navigationMenu { // Use struct for reusability
    string heading;
    vector<string> options;
    void print(int optionNumberPadding, int optionLabelMargin) {
        /*
        optionNumberPadding:
            Distance between option number containers and
            there corresponding label
        */
        /*
        optionLabelMargin:
            Distance between option label containers and 
            their corresponding label number
        */

        cout << right << setw(40) << "SERENDIPITY" << endl << // Print title
            right << setw(50) << heading << endl;

        for (size_t i = 0; i < options.size(); i++) { // Print each option
            cout << right << setw(optionNumberPadding) << i + 1 << "." <<
                right << setw(optionLabelMargin) << "" << 
                left << setw(options[i].size()) << options[i] << endl;
        }
    }
};

#endif // UTILS_H