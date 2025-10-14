/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 10/13/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/
#include "bookinfo.h"

#include <iomanip>
#include <iostream>

#include "utils.h"

void bookInfo(const bookType &book)
{
    clearScreen();
  // Consistent simple header without navigationMenu
  std::cout << "Serendipity Booksellers\n\n"
        << "Book Information\n"
        << std::string(16, '-') << "\n\n";

  // Delegate consistent field formatting to bookType::print
  book.print(std::cout);

    pressEnterToContinue();
}
