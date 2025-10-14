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
#include "utils.h"
#include "reports.h"
#include "invmenu.h"
#include "cashier.h"

using namespace std;

void printMainMenu() {
    navigationMenu mainMenu(
        "Main Menu",
        {
            "Cashier Module",
            "Inventory Database Module",
            "Report Module"
        }
    ); // Initialize main menu

    mainMenu.printWithBack("Exit"); // 0 exits
}

int main() {
    bool loopShouldEnd = false;

    do {
        clearScreen(); // Clear the screen before printing options

        printMainMenu();

        char choice;

        cout << "Choose an option: ";

        cin.get(choice); // Ensure choice is a char

        switch (choice) {
            case '1':
                pressEnterToContinue();
                cashier();
            break;
            case '2':
                pressEnterToContinue();
                invMenu();
            break;
            case '3':
                pressEnterToContinue();
                reports();
            break;
            case '0':
                pressEnterToContinue();
                loopShouldEnd = true; // end loop
                return 0;
            break;
            default:
                // Choice is invalid
                // Set choice to invalid so that loops runs again
                cout << endl << "Invalid input, please try again" << endl;
                pressEnterToContinue();
            break;
        }

    } while (!loopShouldEnd);
    /*
        One option is to use choice to determine whether the loop should end
        and that would save memory, but using a variable instead increases
        readability
    */
}