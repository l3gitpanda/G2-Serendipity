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

static bool readNonEmptyLine(const char* prompt, string& out)
{
    cout << prompt;
    getline(cin >> ws, out);
    if (out.empty()) {
        cout << "Please enter a value.\n";
        return false;
    }
    return true;
}

void cashier()
{
    clearScreen();

    cout << "Serendipity Booksellers\n\n";
    cout << "Cashier Module\n\n";

    string date, isbn, title;
    int qty = 0;
    double unit = 0.0;

    // Date
    while (!readNonEmptyLine("Date: ", date)) {}

    // Quantity (positive integer)
    for (;;)
    {
        cout << "Quantity: ";
        if (cin >> qty && qty > 0) break;
        cout << "Enter a positive whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // ISBN (non-empty)
    while (!readNonEmptyLine("ISBN: ", isbn)) {}

    // Title (non-empty)
    while (!readNonEmptyLine("Title: ", title)) {}

    // Unit Price (non-negative)
    for (;;)
    {
        cout << "Unit Price: ";
        if (cin >> unit && unit >= 0.0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear trailing newline for pause
            break;
        }
        cout << "Enter a non-negative number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

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
    cout << left  << setw(12) << "Price:"    << "$ " << unit     << '\n';
    cout << left  << setw(12) << "Subtotal:" << "$ " << subtotal << '\n';
    cout << left  << setw(12) << "Tax (6%):" << "$ " << tax      << '\n';
    cout << left  << setw(12) << "Total:"    << "$ " << total    << '\n';

    pressEnterToContinue();
}