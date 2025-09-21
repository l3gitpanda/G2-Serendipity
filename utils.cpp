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

using namespace std;

void clearScrean() {
    // Clear the screen
    cout << "\033[H\033[2j" << endl;
}