#include "bookinfo.h"

#include <iomanip>
#include <iostream>

#include "utils.h"

void bookInfo(const bookType &book)
{
    clearScreen();

    std::cout << "Serendipity Booksellers\n\n"
              << "Book Information\n"
              << "----------------\n\n";

    std::cout << "ISBN:             " << book.getISBN() << '\n'
              << "Title:            " << book.getTitle() << '\n'
              << "Author:           " << book.getAuthor() << '\n'
              << "Publisher:        " << book.getPublisher() << '\n'
              << "Date Added:       " << book.getDateAdded() << '\n'
              << "Quantity on Hand: " << book.getQtyOnHand() << '\n'
              << "Wholesale Cost:   " << formatMoney(book.getWholesale()) << '\n'
              << "Retail Price:     " << formatMoney(book.getRetail()) << '\n';

    pressEnterToContinue();
}
