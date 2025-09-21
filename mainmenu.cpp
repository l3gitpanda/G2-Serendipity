/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 9/23/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp cashier.cpp -o serendipity.out
*/

#include <iostream>
#include <iomanip>
#include "utils.h"
#include "reports.h"

using namespace std;

void printMainMenu() {
    int optionLabelPadding = 10;
    /*
    Distance between option label containers and 
    their corresponding label number
    */

    cout << right << setw(40) << "SERENDIPITY" << endl << 
        right << setw(12) << "1:" << 
        right << setw(optionLabelPadding) << "Cashier Module" << endl <<
        right << setw(12) << "2:" << 
        right << setw(optionLabelPadding) << "Inventory Database Module" << endl <<
        right << setw(12) << "3:" << 
        right << setw(optionLabelPadding) << "Report Module" << endl <<
        right << setw(12) << "4:" << 
        right << setw(optionLabelPadding) << "Exit" << endl;
}

int main() {
    bool choiceIsValid = true;

    do {
        clearScreen(); // Clear the screen before printing options

        printMainMenu();

        char choice;

        cout << "Choose and option: ";

        cin.get(choice); // Ensure choice is a char

        switch (choice) {
            case '1':
            pressEnterToContinue();
            // cashier()
            break;
            case '2':
            pressEnterToContinue();
            // invmenu()
            break;
            case '3':
            pressEnterToContinue();
            reports();
            break;
            case '4':
            pressEnterToContinue();
            return;
            break;
            default:
            choiceIsValid = false; 
            // Set choice to invalid so that loops runs again
            cout << endl << "Invalid input, please try again" << endl;
            pressEnterToContinue();
            break;
        }

    } while (!choiceIsValid);
}