/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 11/16/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/
/**
 * @file bookType.h
 * @brief Definitions for the bookType class used by the Serendipity inventory
 * manager.
 *
 * The bookType class models a single book in inventory including identifying
 * metadata, quantities and pricing. Invariants: `qtyOnHand >= 0`,
 * `wholesale >= 0.0`, `retail >= 0.0`.
 *
 * @section uml_bookType BookType UML Overview
 * - Stereotype: «entity»
 * - Responsibilities: represent a single book record and enforce basic
 *   non-negative constraints on quantity and pricing.
 * - Collaborators: used by inventory menus, cashier, and reporting modules.
 */

#ifndef BOOKTYPE_H
#define BOOKTYPE_H
#include <iosfwd>
#include <string>

/**
 * @class bookType
 * @brief Represents a book in inventory.
 *
 * Fields:
 * - `isbn`: unique identifier for the book.
 * - `title`, `author`, `publisher`, `dateAdded`: metadata strings.
 * - `qtyOnHand`: number of copies available (non-negative).
 * - `wholesale`, `retail`: pricing values (non-negative).
 *
 * Constructors, copy/move operations and accessors are provided. Invariants:
 * `qtyOnHand >= 0`, `wholesale >= 0.0`, `retail >= 0.0`.
 */
class bookType {
public:
  bookType();
  /**
   * @brief Full constructor for a book record.
   * @param isbn Book ISBN/identifier.
   * @param title Title of the book.
   * @param author Author name.
   * @param publisher Publisher name.
   * @param dateAdded Date the book was added to inventory (string form).
   * @param qtyOnHand Quantity on hand. Must be >= 0.
   * @param wholesale Wholesale cost. Must be >= 0.0.
   * @param retail Retail price. Must be >= 0.0.
   * @pre `qtyOnHand >= 0`, `wholesale >= 0.0`, `retail >= 0.0`.
   * @post The book object contains the provided values.
   */
  bookType(const std::string &isbn, const std::string &title,
           const std::string &author, const std::string &publisher,
           const std::string &dateAdded, int qtyOnHand,
           double wholesale, double retail);
  bookType(const bookType &other);
  bookType(bookType &&other) noexcept;
  bookType &operator=(const bookType &other);
  bookType &operator=(bookType &&other) noexcept;
  ~bookType();

  /**
   * @brief Set the ISBN value.
   * @param isbnValue New ISBN string.
   * @post `getISBN()` will return `isbnValue`.
   */
  void setISBN(const std::string &isbnValue);
  /**
   * @brief Set the title value.
   * @param titleValue New title string.
   * @post `getTitle()` will return `titleValue`.
   */
  void setTitle(const std::string &titleValue);
  /**
   * @brief Set the author value.
   * @param authorValue New author string.
   * @post `getAuthor()` will return `authorValue`.
   */
  void setAuthor(const std::string &authorValue);
  /**
   * @brief Set the publisher value.
   * @param publisherValue New publisher string.
   * @post `getPublisher()` will return `publisherValue`.
   */
  void setPublisher(const std::string &publisherValue);
  /**
   * @brief Set the date the book was added.
   * @param dateValue New date string.
   * @post `getDateAdded()` will return `dateValue`.
   */
  void setDateAdded(const std::string &dateValue);
  /**
   * @brief Set the quantity on hand.
   * @param qtyValue New quantity value.
   * @pre `qtyValue >= 0`.
   * @post `getQtyOnHand()` will return `qtyValue`.
   */
  void setQtyOnHand(int qtyValue);
  /**
   * @brief Set the wholesale price.
   * @param wholesaleValue New wholesale price.
   * @pre `wholesaleValue >= 0.0`.
   * @post `getWholesale()` will return `wholesaleValue`.
   */
  void setWholesale(double wholesaleValue);
  /**
   * @brief Set the retail price.
   * @param retailValue New retail price.
   * @pre `retailValue >= 0.0`.
   * @post `getRetail()` will return `retailValue`.
   */
  void setRetail(double retailValue);

  /**
   * @brief Get the ISBN string.
   * @return const reference to the ISBN.
   */
  const std::string &getISBN() const;
  const std::string &getTitle() const;
  const std::string &getAuthor() const;
  const std::string &getPublisher() const;
  const std::string &getDateAdded() const;
  int getQtyOnHand() const;
  double getWholesale() const;
  double getRetail() const;

  static int getBookCount();

  /**
   * @brief Compare two bookType objects by ISBN (and other fields as needed).
   * @param other Book to compare with.
   * @return true if the two objects are considered equal.
   */
  bool equals(const bookType &other) const;
  /**
   * @brief Print a human-readable representation to the given stream.
   * @param os Output stream to write to.
   */
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
