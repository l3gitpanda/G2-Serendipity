/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 11/16/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/
#include "bookType.h"
#include "utils.h"

#include <iomanip>
#include <iostream>

int bookType::bookCount = 0;

bookType::bookType()
{
  ++bookCount;
}

bookType::bookType(const std::string &isbnValue, const std::string &titleValue,
                   const std::string &authorValue, const std::string &publisherValue,
                   const std::string &dateValue, int qtyValue,
                   double wholesaleValue, double retailValue)
  : isbn(isbnValue), title(titleValue), author(authorValue), publisher(publisherValue),
    dateAdded(dateValue), qtyOnHand(qtyValue), wholesale(wholesaleValue), retail(retailValue)
{
  ++bookCount;
}

bookType::bookType(const bookType &other)
  : isbn(other.isbn), title(other.title), author(other.author),
    publisher(other.publisher), dateAdded(other.dateAdded),
    qtyOnHand(other.qtyOnHand), wholesale(other.wholesale), retail(other.retail)
{
  ++bookCount;
}

bookType::bookType(bookType &&other) noexcept
  : isbn(other.isbn), title(other.title), author(other.author),
    publisher(other.publisher), dateAdded(other.dateAdded),
    qtyOnHand(other.qtyOnHand), wholesale(other.wholesale), retail(other.retail)
{
  ++bookCount;
}

bookType &bookType::operator=(const bookType &other)
{
  if (this != &other)
  {
    isbn       = other.isbn;
    title      = other.title;
    author     = other.author;
    publisher  = other.publisher;
    dateAdded  = other.dateAdded;
    qtyOnHand  = other.qtyOnHand;
    wholesale  = other.wholesale;
    retail     = other.retail;
  }
  return *this;
}

bookType &bookType::operator=(bookType &&other) noexcept
{
  if (this != &other)
  {
    isbn       = other.isbn;
    title      = other.title;
    author     = other.author;
    publisher  = other.publisher;
    dateAdded  = other.dateAdded;
    qtyOnHand  = other.qtyOnHand;
    wholesale  = other.wholesale;
    retail     = other.retail;
  }
  return *this;
}

bookType::~bookType()
{
  --bookCount;
}

void bookType::setISBN(const std::string &isbnValue) { isbn = isbnValue; }
void bookType::setTitle(const std::string &titleValue) { title = titleValue; }
void bookType::setAuthor(const std::string &authorValue) { author = authorValue; }
void bookType::setPublisher(const std::string &publisherValue) { publisher = publisherValue; }
void bookType::setDateAdded(const std::string &dateValue) { dateAdded = dateValue; }
void bookType::setQtyOnHand(int qtyValue) { qtyOnHand = qtyValue; }
void bookType::setWholesale(double wholesaleValue) { wholesale = wholesaleValue; }
void bookType::setRetail(double retailValue) { retail = retailValue; }

const std::string &bookType::getISBN() const { return isbn; }
const std::string &bookType::getTitle() const { return title; }
const std::string &bookType::getAuthor() const { return author; }
const std::string &bookType::getPublisher() const { return publisher; }
const std::string &bookType::getDateAdded() const { return dateAdded; }
int bookType::getQtyOnHand() const { return qtyOnHand; }
double bookType::getWholesale() const { return wholesale; }
double bookType::getRetail() const { return retail; }

int bookType::getBookCount()
{
  return bookCount;
}

bool bookType::equals(const bookType &other) const
{
  return isbn == other.isbn && title == other.title && author == other.author &&
         publisher == other.publisher && dateAdded == other.dateAdded &&
         qtyOnHand == other.qtyOnHand && wholesale == other.wholesale &&
         retail == other.retail;
}

void bookType::print(std::ostream &os) const
{
  // Consistent aligned labels and money formatting
  std::ios::fmtflags flags = os.flags();
  std::streamsize precision = os.precision();

  auto line = [&](const std::string &label, const std::string &value) {
    std::ios::fmtflags f = os.flags();
    os << std::left << std::setw(18) << (label + ":") << value << '\n';
    os.flags(f);
  };

  line("ISBN", isbn);
  line("Title", title);
  line("Author", author);
  line("Publisher", publisher);
  line("Date Added", dateAdded);
  {
    std::ios::fmtflags f = os.flags();
    os << std::left << std::setw(18) << "Quantity on Hand:" << qtyOnHand << '\n';
    os.flags(f);
  }
  line("Wholesale Cost", formatMoney(wholesale));
  line("Retail Price", formatMoney(retail));

  os.flags(flags);
  os.precision(precision);
}