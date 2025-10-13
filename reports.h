/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 9/23/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp -o serendipity.out
*/

/***************************/
// Minimum Stylesheet:
// Programmer: Ian Bagherzadeh
// Assignment: G2 — Serendipity (Reports menu & stubs)
// Purpose: Reports submenu navigation + stubs
// Due Date:
/***************************/
#ifndef REPORTS_H
#define REPORTS_H

void reports();

void repListing();
void repWholesale();
void repRetail();
void repQty();
void repCost();
void repAge();

#endif
