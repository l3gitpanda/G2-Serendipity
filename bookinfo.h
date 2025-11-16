/**
 * @file bookinfo.h
 * @brief Helper to print or display detailed information about a bookType.
 */

#ifndef BOOKINFO_H
#define BOOKINFO_H

#include "bookType.h"

/**
 * @brief Display human-readable information for the provided book.
 * @param book The book record to display.
 * @pre `book` is a valid bookType object.
 * @post A textual representation of `book` has been written to stdout
 *       (or the program's configured output stream).
 */
void bookInfo(const bookType &book);

#endif // BOOKINFO_H
