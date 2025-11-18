/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 11/16/2025
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

    mainMenu.printWithBack("Exit");
}

int main() {
    bool loopShouldEnd = false;

    do {
        clearScreen(); // Clear the screen before printing options

        printMainMenu();

        std::string input;

        cout << "Choose an option: ";
        if (!std::getline(cin, input)) {
            return 0;
        }

        input = trim(input);
        int selection = -1;
        if (!parseNonNegativeInt(input, selection))
        {
            cout << endl << "Invalid input, please try again" << endl;
            pressEnterToContinue();
            continue;
        }

        const int returnIndex = 3 + 1; // 3 menu items + return
        if (selection == returnIndex)
        {
            pressEnterToContinue();
            return 0;
        }

        switch (selection) {
            case 1:
                pressEnterToContinue();
                cashier();
            break;
            case 2:
                pressEnterToContinue();
                invMenu();
            break;
            case 3:
                pressEnterToContinue();
                reports();
            break;
            default:
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