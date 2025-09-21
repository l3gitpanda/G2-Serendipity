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
        right << setw(optionLabelPadding) << "View Listing" << endl <<
        right << setw(optionNumberPadding) << "2:" << 
        right << setw(optionLabelPadding) << "View Wholesale" << endl <<
        right << setw(optionNumberPadding) << "3:" << 
        right << setw(optionLabelPadding) << "View Retail" << endl <<
        right << setw(optionNumberPadding) << "4:" << 
        right << setw(optionLabelPadding) << "View quantity" << endl <<
        right << setw(optionNumberPadding) << "5:" << 
        right << setw(optionLabelPadding) << "View Cost" << endl <<
        right << setw(optionNumberPadding) << "6:" << 
        right << setw(optionLabelPadding) << "View Age" << endl <<
        right << setw(optionNumberPadding) << "7:" << 
        right << setw(optionLabelPadding) << "Go back to main menu" << endl;
        
}

void reports() {

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

