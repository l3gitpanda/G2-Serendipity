/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 10/10/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp -o serendipity.out
*/

#ifndef BOOKINFO_H
#define BOOKINFO_H

#include "bookType.h"

void bookInfo(const BookType &book);

#endif // BOOKINFO_H
