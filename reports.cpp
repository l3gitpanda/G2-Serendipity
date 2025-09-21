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
#include <iomanip>
#include "utils.h"

using namespace std;

void printReportsMenu() {
    navigationMenu reportsMenu(
        "Reports Menu", 
        {
            "Inventory Listing",
            "Inventory Wholesale Value",
            "Inventory Retail Value",
            "Listing by Quantity",
            "Listing by Cost",
            "Listing by Age",
            "Return to Main Menu"
        }
    ); // Initialize menu

    reportsMenu.print(12, 5); // Print menu
}

void reports() {
    bool loopShouldEnd = false;

    do {
        clearScreen(); // Clear the screen before printing options

        printReportsMenu();

        char choice;

        cout << "Choose an option: ";

        cin.get(choice); // Ensure choice is a char

        switch (choice) {
            case '1':
                repListing();
                pressEnterToContinue();
            break;
            case '2':
                repWholesale();
                pressEnterToContinue();
            break;
            case '3':
                repRetail();
                pressEnterToContinue();
            break;
            case '4':
                repQty();
                pressEnterToContinue();
            break;
            case '5':
                repCost();
                pressEnterToContinue();
            break;
            case '6':
                repAge();
                pressEnterToContinue();
            break;
            case '7':
                pressEnterToContinue();
                loopShouldEnd = true; // End loop
                return;
            break;
            default:
                // Choice is invalid
                // Set choice to invalid so that loops runs again
                cout << endl << "Invalid input, please try again" << endl;
                pressEnterToContinue();
            break;
        }

    } while (!loopShouldEnd);
}

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

