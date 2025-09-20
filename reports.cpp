/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 9/23/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp cashier.cpp -o serendipity.out
*/

#include <iostream>
#include "reports.h"

using namespace std;

void repListing() {
    cout << "You've chosen a listing";
}

void repWholesale() {
    cout << "You've chosen Wholesale";
}

void repRetail() {
    cout << "You've chosen Retail";
}

void repQty() {
    cout << "You've chosen Quantity";
}

void repCost() {
    cout << "You've chosen a Cost";
}

void repAge() {
    cout << "You've chosen Age";
}

