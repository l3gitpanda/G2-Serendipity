/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 9/23/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp book.cpp cashier.cpp -o serendipity.out
*/
#include <iostream>
#include "book.h"

using namespace std;

bool validateDateInput(string date) {
    return true;
}

string sanitizeDateInput(string date) {
    return date;
}