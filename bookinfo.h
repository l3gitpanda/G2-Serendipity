/**
 * @file bookinfo.h
 * @brief Helper to print or display detailed information about a bookType.
 *
 * @section uml BookInfo UML Overview
 * - Stereotype: «boundary»
 * - Responsibilities: present a single `bookType` instance in a
 *   human-friendly format.
 * - Collaborators: consumes `bookType` and uses standard output streams.
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
