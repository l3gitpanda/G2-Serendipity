/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 9/23/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp book.cpp cashier.cpp -o serendipity.out
*/

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include "cashier.h"
#include "utils.h"
using namespace std;

void cashier()
{
    clearScreen();

    cout << "Serendipity Booksellers\n\n";
    cout << "Cashier Module\n\n";


    string date, isbn, title;
    int qty = 0;
    double unit = 0.0;

    cout << "Date: ";
	getline(cin >> ws, date);


    cout << "Quantity: ";
    cin >> qty;

    cout << "ISBN: ";
    cin >> isbn;

    cout << "Title: ";
    getline(cin >> ws, title);

    cout << "Unit Price: ";
    cin >> unit;

    const double TAX_RATE = 0.06;
    double subtotal = qty * unit;
    double tax = subtotal * TAX_RATE;
    double total = subtotal + tax;

    cout << "\n\nReceipt\n";
    cout << "------------------------------\n";
    cout << left  << setw(12) << "Date:"   << date  << '\n';
    cout << left  << setw(12) << "Qty:"    << qty   << '\n';
    cout << left  << setw(12) << "ISBN:"   << isbn  << '\n';
    cout << left  << setw(12) << "Title:"  << title << '\n';

    cout << fixed << setprecision(2);
    cout << left  << setw(12) << "Price:"    << "$ " << setw(0) << unit     << '\n';
    cout << left  << setw(12) << "Subtotal:" << "$ " << subtotal << '\n';
    cout << left  << setw(12) << "Tax (6%):" << "$ " << tax      << '\n';
    cout << left  << setw(12) << "Total:"    << "$ " << total    << '\n';

    pressEnterToContinue();
}