/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 10/12/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp -o serendipity.out
*/
#ifndef BOOKTYPE_H
#define BOOKTYPE_H
#include <iostream>
#include <string>

using namespace std;

class BookType {
  public:
    BookType();
    BookType(const string &bookTitle, const string &isbn, const string &author, const string &publisher,
        const string &dateAdded, int qtyOnHand, double wholesale, double retail);
    BookType(const BookType &other);
    BookType(BookType &&other) noexcept;
    BookType &operator=(const BookType &other);
    BookType &operator=(BookType &&other) noexcept;
    ~BookType();

    // Getters
    string getBookTitle() const;
    string getIsbn() const;
    string getAuthor() const;
    string getPublisher() const;
    string getDateAdded() const;
    int getQtyOnHand() const;
    double getWholesale() const;
    double getRetail() const;

    // Setters
    void setBookTitle(const string &title);
    void setIsbn(const string &isbn);
    void setAuthor(const string &author);
    void setPublisher(const string &publisher);
    void setDateAdded(const string &date);
    void setQtyOnHand(int qty);
    void setWholesale(double wholesale);
    void setRetail(double retail);

    static int getBookCount();

  private:
    string bookTitle;
    string isbn;
    string author;
    string publisher;
    string dateAdded;
    int qtyOnHand{};
    double wholesale{};
    double retail{};

    static int bookCount;
};

#endif // BOOKTYPE_H
