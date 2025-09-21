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
    int optionNumberPadding = 12;
    /*
    Distance between option number containers and
    there corresponding label
    */
    int optionLabelPadding = 10;
    /*
    Distance between option label containers and 
    their corresponding label number
    */

    cout << right << setw(40) << "SERENDIPITY" << endl << 
        right << setw(optionNumberPadding) << "1:" << 
        right << setw(optionLabelPadding) << "Inventory Listing" << endl <<
        right << setw(optionNumberPadding) << "2:" << 
        right << setw(optionLabelPadding) << "Inventory Wholesale Value" << endl <<
        right << setw(optionNumberPadding) << "3:" << 
        right << setw(optionLabelPadding) << "Inventory Retail Value" << endl <<
        right << setw(optionNumberPadding) << "4:" << 
        right << setw(optionLabelPadding) << "Listing by Quantity" << endl <<
        right << setw(optionNumberPadding) << "5:" << 
        right << setw(optionLabelPadding) << "Listing by Cost" << endl <<
        right << setw(optionNumberPadding) << "6:" << 
        right << setw(optionLabelPadding) << "Listing by Age" << endl <<
        right << setw(optionNumberPadding) << "7:" << 
        right << setw(optionLabelPadding) << "Return to Main Menu" << endl;
        
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
                pressEnterToContinue();
                repListing();
            break;
            case '2':
                pressEnterToContinue();
                repWholesale();
            break;
            case '3':
                pressEnterToContinue();
                repRetail();
            break;
            case '4':
                pressEnterToContinue();
                repQty();
            break;
            case '5':
                pressEnterToContinue();
                repCost();
            break;
            case '6':
                pressEnterToContinue();
                repAge();
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

