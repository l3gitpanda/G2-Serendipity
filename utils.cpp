/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 9/23/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp cashier.cpp -o serendipity.out
*/

#include <iostream>
#include "utils.h"
#include <limits>

using namespace std;

void clearScreen() {
    // Clear the screen
    cout << "\033[2J\033[1;1H" << endl;
}

void pressAnyKeyToContinue() {
    cout << "Press any key to continue...";
    cin.ignore(numeric_limits<std::streamsize>::max());
    // Ignore next input
}