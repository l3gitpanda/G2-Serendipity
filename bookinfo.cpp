#include "bookinfo.h"

#include <iomanip>
#include <iostream>

using namespace std;

void bookInfo(const BookType &book)
{
  cout << "Serendipity Booksellers\n";
  cout << "Book Information\n\n";

  cout << left << setw(18) << "ISBN:" << book.getIsbn() << '\n';
  cout << left << setw(18) << "Title:" << book.getBookTitle() << '\n';
  cout << left << setw(18) << "Author:" << book.getAuthor() << '\n';
  cout << left << setw(18) << "Publisher:" << book.getPublisher() << '\n';
  cout << left << setw(18) << "Date Added:" << book.getDateAdded() << '\n';
  cout << left << setw(18) << "Quantity On Hand:" << book.getQtyOnHand() << '\n';

  cout << fixed << setprecision(2);
  cout << left << setw(18) << "Wholesale Cost:" << '$' << book.getWholesale() << '\n';
  cout << left << setw(18) << "Retail Price:" << '$' << book.getRetail() << '\n';
}
