/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 9/23/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp -o serendipity.out
*/

#include <iostream>
#include <string>
#include "invmenu.h"
#include "utils.h"
using namespace std;

static void printInvMenu()
{
  cout << "Serendipity Booksellers\n\n";
  cout << "Inventory Database\n\n";
  cout << "1. Look Up a Book\n";
  cout << "2. Add a Book\n";
  cout << "3. Edit a Book's Record\n";
  cout << "4. Delete a Book\n";
  cout << "5. Return to the Main Menu\n\n";
  cout << "Enter Your Choice: ";
}

void invMenu()
{
  bool running = true;

  while (running)
  {
    clearScreen();
    printInvMenu();

    string input;
    getline(cin, input);

    if (input.size() != 1 || input[0] < '1' || input[0] > '5')
    {
      cout << "\nPlease enter a number in the range 1 – 5.\n";
      pressEnterToContinue();
      continue;
    }

    switch (input[0])
    {
      case '1':
        clearScreen();
        lookUpBook();
        pressEnterToContinue();
        break;
      case '2':
        clearScreen();
        addBook();
        pressEnterToContinue();
        break;
      case '3':
        clearScreen();
        editBook();
        pressEnterToContinue();
        break;
      case '4':
        clearScreen();
        deleteBook();
        pressEnterToContinue();
        break;
      case '5':
        running = false; // Return to Main Menu
        break;
    }
  }
}

// Inventory stubs (navigation-only)
void lookUpBook()  { cout << "You selected Look Up Book.\n"; }
void addBook()     { cout << "You selected Add Book.\n"; }
void editBook()    { cout << "You selected Edit Book.\n"; }
void deleteBook()  { cout << "You selected Delete Book.\n"; }