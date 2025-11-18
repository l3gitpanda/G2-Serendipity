/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 11/16/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/
#include "bookinfo.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include "utils.h"

void bookInfo(const bookType &book)
{
  clearScreen();

  // Local framed output (no navigationMenu) matching cashier style
  constexpr int FRAME_W = 80;
  constexpr int INSIDE  = 78;
  auto border = [&]() { std::cout << std::string(FRAME_W, '-') << '\n'; };
  auto framed = [&](const std::string &s) {
    std::string t = s;
    if ((int)t.size() < INSIDE) t += std::string(INSIDE - (int)t.size(), ' ');
    else if ((int)t.size() > INSIDE) t = t.substr(0, INSIDE);
    std::cout << '-' << t << "-\n";
  };
  auto blank = [&]() { framed(""); };

  border();
  framed("Serendipity Booksellers");
  blank();
  framed("Book Information");
  blank();

  // Render the book fields using the standardized print, then frame line-by-line
  std::ostringstream oss;
  book.print(oss);
  std::istringstream iss(oss.str());
  std::string line;
  while (std::getline(iss, line))
  {
    if (!line.empty() && line.back() == '\r') line.pop_back();
    framed(line);
  }
  border();

  pressEnterToContinue();
}
