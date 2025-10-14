/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 10/14/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/

#include <iostream>
#include <string>
#include "reports.h"
#include "utils.h"
using namespace std;

static void printReportsMenu()
{
    navigationMenu menu{
        "Reports",
        {
            "Inventory Listing",
            "Inventory Wholesale Value",
            "Inventory Retail Value",
            "Listing by Quantity",
            "Listing by Cost",
            "Listing by Age"
        }
    };
    menu.printWithBack("Return to Main Menu");
}

void reports()
{
    bool running = true;

    while (running)
    {
        clearScreen();
    printReportsMenu();
    cout << "Enter Your Choice: ";

        string input;
        getline(cin, input);

        if (input.size() != 1 || (input[0] != '0' && (input[0] < '1' || input[0] > '6')))
        {
            cout << "\nPlease enter 0 or a number in the range 1 - 6.\n";
            pressEnterToContinue();
            continue;
        }

        switch (input[0])
        {
            case '1':
                clearScreen();
                repListing();
                pressEnterToContinue();
                break;
            case '2':
                clearScreen();
                repWholesale();
                pressEnterToContinue();
                break;
            case '3':
                clearScreen();
                repRetail();
                pressEnterToContinue();
                break;
            case '4':
                clearScreen();
                repQty();
                pressEnterToContinue();
                break;
            case '5':
                clearScreen();
                repCost();
                pressEnterToContinue();
                break;
            case '6':
                clearScreen();
                repAge();
                pressEnterToContinue();
                break;
            case '0':
                pressEnterToContinue();
                running = false;
                break;
        }
    }
}

// ----- Report stubs -----
void repListing()   { cout << "Inventory Listing selected.\n"; }
void repWholesale() { cout << "Inventory Wholesale Value selected.\n"; }
void repRetail()    { cout << "Inventory Retail Value selected.\n"; }
void repQty()       { cout << "Listing by Quantity selected.\n"; }
void repCost()      { cout << "Listing by Cost selected.\n"; }
void repAge()       { cout << "Listing by Age selected.\n"; }