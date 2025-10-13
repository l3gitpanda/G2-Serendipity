/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 10/10/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp -o serendipity.out
*/

#include "bookinfo.h"

#include <iomanip>
#include <iostream>

using namespace std;

void bookInfo(const BookType &book)
{
  cout << "Book Information\n";
  cout << "-----------------\n";
  cout << left << setw(20) << "Title:" << book.getBookTitle() << '\n';
  cout << left << setw(20) << "ISBN:" << book.getIsbn() << '\n';
  cout << left << setw(20) << "Author:" << book.getAuthor() << '\n';
  cout << left << setw(20) << "Publisher:" << book.getPublisher() << '\n';
  cout << left << setw(20) << "Date Added:" << book.getDateAdded() << '\n';
  cout << left << setw(20) << "Quantity on Hand:" << book.getQtyOnHand() << '\n';
  cout << left << setw(20) << "Wholesale Cost:" << '$' << fixed << setprecision(2) << book.getWholesale() << '\n';
  cout << left << setw(20) << "Retail Price:" << '$' << fixed << setprecision(2) << book.getRetail() << '\n';
}
