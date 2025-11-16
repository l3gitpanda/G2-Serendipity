/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 10/14/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/

/**
 * @file cashier.h
 * @brief Cashier mode entry point for processing sales and updating inventory.
 */

#ifndef CASHIER_H
#define CASHIER_H

/**
 * @brief Run cashier workflow: sell books, compute totals, and update stock.
 * @pre Inventory data should be loaded and accessible by the application.
 * @post Inventory quantities may be decreased and sales information recorded.
 * @note Implementation details (file/storage) are defined in `cashier.cpp`.
 */
void cashier();

#endif