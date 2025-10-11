/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 9/23/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp -o serendipity.out
*/
#include "bookType.h"
#include <utility>

BookType::BookType() = default;

BookType::BookType(const string &bookTitle, const string &isbn, const string &author, const string &publisher,
           const string &dateAdded, int qtyOnHand, double wholesale, double retail)
  : bookTitle(bookTitle), isbn(isbn), author(author), publisher(publisher), dateAdded(dateAdded),
    qtyOnHand(qtyOnHand), wholesale(wholesale), retail(retail) {}

// Getters
string BookType::getBookTitle() const { return bookTitle; }
string BookType::getIsbn() const { return isbn; }
string BookType::getAuthor() const { return author; }
string BookType::getPublisher() const { return publisher; }
string BookType::getDateAdded() const { return dateAdded; }
int BookType::getQtyOnHand() const { return qtyOnHand; }
double BookType::getWholesale() const { return wholesale; }
double BookType::getRetail() const { return retail; }

// Setters
void BookType::setBookTitle(const string &title) { bookTitle = title; }
void BookType::setIsbn(const string &isbn_) { isbn = isbn_; }
void BookType::setAuthor(const string &author_) { author = author_; }
void BookType::setPublisher(const string &publisher_) { publisher = publisher_; }
void BookType::setDateAdded(const string &date) { dateAdded = date; }
void BookType::setQtyOnHand(int qty) { qtyOnHand = qty; }
void BookType::setWholesale(double w) { wholesale = w; }
void BookType::setRetail(double r) { retail = r; }