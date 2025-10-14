/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 10/14/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include "cashier.h"
#include "utils.h"
using namespace std;

static string money(double v)
{
    if (v < 0) v = 0;
    long long cents = (long long)(v * 100.0 + 0.5);
    long long dollars = cents / 100;
    int frac = (int)(cents % 100);
    string s = "$ " + to_string(dollars) + ".";
    if (frac < 10) s += "0";
    s += to_string(frac);
    return s;
}

static string squeezeForWidth(string s, int width)
{
    if ((int)s.size() == width + 1 && s.size() >= 2 && s[0] == '$' && s[1] == ' ')
        return string("$") + s.substr(2);
    return s;
}

static string clampCellRight(const string& s, int width, bool keepDollar)
{
    if ((int)s.size() <= width) return s;
    if (keepDollar && !s.empty() && s[0] == '$' && width >= 2)
    {
        string tail = s.substr((int)s.size() - (width - 1));
        return string("$") + tail;
    }
    return s.substr((int)s.size() - width);
}

void cashier()
{
    clearScreen();

    string date, isbn, title;
    int qty = 0;
    double unit = 0.0;

    cout << "Serendipity Book Sellers\n\n";
    cout << "Cashier Module\n\n";

    cout << "Date: ";
    getline(cin >> ws, date);
    while (date.empty()) { cout << "Please enter a date: "; getline(cin >> ws, date); }

    for (;;)
    {
        cout << "Quantity: ";
        if (cin >> qty && qty > 0) break;
        cout << "Enter a positive whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "ISBN: ";
    getline(cin >> ws, isbn);
    while (isbn.empty()) { cout << "Please enter an ISBN: "; getline(cin >> ws, isbn); }

    cout << "Title: ";
    getline(cin >> ws, title);
    while (title.empty()) { cout << "Please enter a title: "; getline(cin >> ws, title); }

    for (;;)
    {
        cout << "Unit Price: ";
        if (cin >> unit && unit >= 0.0) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); break; }
        cout << "Enter a non-negative number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    const double TAX_RATE = 0.06;
    double lineTotal  = qty * unit;
    double subtotal   = lineTotal;
    double tax        = subtotal * TAX_RATE;
    double grandTotal = subtotal + tax;

    const int FRAME_W = 80;
    const int INSIDE  = 78;
    const int COL_QTY   = 1;
    const int COL_ISBN  = 6;
    const int COL_TITLE = 20;
    const int COL_PRICE = 56;
    const int COL_TOTAL = 68;

    auto border = [&]() { cout << string(FRAME_W, '-') << '\n'; };
    auto framed = [&](const string& s) {
        string t = s;
        if ((int)t.size() < INSIDE) t += string(INSIDE - (int)t.size(), ' ');
        else if ((int)t.size() > INSIDE) t = t.substr(0, INSIDE);
        cout << '-' << t << "-\n";
    };
    auto blank = [&]() { framed(""); };
    auto put = [&](string& row, int col, const string& text) {
        for (size_t i = 0; i < text.size() && col + (int)i < INSIDE; ++i) row[col + (int)i] = text[i];
    };
    auto rightPut = [&](string& row, int col, int width, const string& text) {
        string cell = text;
        if ((int)cell.size() > width) cell = cell.substr((int)cell.size() - width);
        int start = col + width - (int)cell.size();
        if (start < col) start = col;
        put(row, start, cell);
    };

    border();
    framed("Serendipity Book Sellers");
    blank();
    framed("Date: " + date);
    blank();

    {
        string row(INSIDE, ' ');
        put(row, COL_QTY,   "Qty");
        put(row, COL_ISBN,  "ISBN");
        put(row, COL_TITLE, "Title");
        put(row, COL_PRICE, "Price");
        put(row, COL_TOTAL, "Total");
        framed(row);
    }
    border();

    {
        string row(INSIDE, ' ');
        string q = to_string(qty);
        if ((int)q.size() < 4) q = string(4 - (int)q.size(), ' ') + q;
        put(row, COL_QTY, q);

        string isbnCell = isbn.substr(0, 14);
        put(row, COL_ISBN, isbnCell);

        string titleCell = title.substr(0, 38);
        put(row, COL_TITLE, titleCell);

        string priceCell = money(unit);
        priceCell = squeezeForWidth(priceCell, 11);
        priceCell = clampCellRight(priceCell, 11, true);
        put(row, COL_PRICE, priceCell);

        string totalCell = money(lineTotal);
        totalCell = squeezeForWidth(totalCell, 10);
        totalCell = clampCellRight(totalCell, 10, true);
        rightPut(row, COL_TOTAL, 10, totalCell);

        framed(row);
    }

    blank();
    blank();

    auto totalsRow = [&](const string& label, const string& amt) {
        string row(INSIDE, ' ');
        put(row, COL_PRICE, label);
        string cell = squeezeForWidth(amt, 10);
        cell = clampCellRight(cell, 10, true);
        rightPut(row, COL_TOTAL, 10, cell);
        framed(row);
    };

    totalsRow("Subtotal", money(subtotal));
    totalsRow("Tax",      money(tax));
    totalsRow("Total",    money(grandTotal));
    border();

    pressEnterToContinue();
}