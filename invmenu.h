/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 10/14/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/

#ifndef INV_MENU_H
#define INV_MENU_H

void invMenu();

int lookUpBook();
void addBook();
void editBook();
void deleteBook();

#endif // INV_MENU_H
