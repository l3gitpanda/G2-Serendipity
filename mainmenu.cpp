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
    navigationMenu mainMenu(
        "Main Menu",
        {
            "Cashier Module",
            "Inventory Database Module",
            "Report Module",
            "Exit"
        }
    ); // Initialize main menu

    mainMenu.print(); // Print main menu
}

int main() {
    bool loopShouldEnd = false;

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