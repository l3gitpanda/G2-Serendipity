#include "bookinfo.h"

#include <iomanip>
#include <iostream>

using namespace std;

namespace
{
  void printHeader()
  {
    cout << left << setw(18) << "DATABASE SIZE:" << 20 << '\n';
    cout << left << setw(18) << "CURRENT COUNT:" << BookType::getBookCount() << "\n\n";
  }
}

void bookInfo(const BookType &book)
{
  printHeader();

  cout << "Book Info" << '\n';
  cout << "---------" << '\n';

  cout << left << setw(20) << "Title:" << book.getBookTitle() << '\n';
  cout << left << setw(20) << "ISBN:" << book.getIsbn() << '\n';
  cout << left << setw(20) << "Author:" << book.getAuthor() << '\n';
  cout << left << setw(20) << "Publisher:" << book.getPublisher() << '\n';
  cout << left << setw(20) << "Date Added:" << book.getDateAdded() << '\n';
  cout << left << setw(20) << "Quantity on Hand:" << book.getQtyOnHand() << '\n';
  cout << left << setw(20) << "Wholesale Cost:" << '$' << fixed << setprecision(2) << book.getWholesale() << '\n';
  cout << left << setw(20) << "Retail Price:" << '$' << fixed << setprecision(2) << book.getRetail() << '\n';
}
