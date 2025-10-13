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
#include <iosfwd>
#include <string>

class bookType {
public:
  bookType();
  bookType(const std::string &isbn, const std::string &title,
           const std::string &author, const std::string &publisher,
           const std::string &dateAdded, int qtyOnHand,
           double wholesale, double retail);
  bookType(const bookType &other);
  bookType(bookType &&other) noexcept;
  bookType &operator=(const bookType &other);
  bookType &operator=(bookType &&other) noexcept;
  ~bookType();

  void setISBN(const std::string &isbnValue);
  void setTitle(const std::string &titleValue);
  void setAuthor(const std::string &authorValue);
  void setPublisher(const std::string &publisherValue);
  void setDateAdded(const std::string &dateValue);
  void setQtyOnHand(int qtyValue);
  void setWholesale(double wholesaleValue);
  void setRetail(double retailValue);

  const std::string &getISBN() const;
  const std::string &getTitle() const;
  const std::string &getAuthor() const;
  const std::string &getPublisher() const;
  const std::string &getDateAdded() const;
  int getQtyOnHand() const;
  double getWholesale() const;
  double getRetail() const;

  static int getBookCount();

  bool equals(const bookType &other) const;
  void print(std::ostream &os) const;

private:
  std::string isbn;
  std::string title;
  std::string author;
  std::string publisher;
  std::string dateAdded;
  int qtyOnHand{0};
  double wholesale{0.0};
  double retail{0.0};

  static int bookCount;
};

#endif // BOOKTYPE_H
