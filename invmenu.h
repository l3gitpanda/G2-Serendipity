/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 11/16/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/

/**
 * @file invmenu.h
 * @brief Inventory menu functions for the Serendipity application.
 *
 * @section uml_invMenu InvMenu UML Overview
 * - Stereotype: «control»
 * - Responsibilities: drive inventory-related workflows such as lookup,
 *   add, edit, and delete operations.
 * - Collaborators: uses `bookType` entities and utility functions for
 *   formatting, validation, and user interaction.
 */

#ifndef INV_MENU_H
#define INV_MENU_H

/**
 * @brief Launch the inventory management menu.
 * @post Control returns to the main menu when the user exits the inventory menu.
 */
void invMenu();

/**
 * @brief Look up a book by ISBN or title.
 * @return index or id of the found book, or -1 if not found.
 * @pre Inventory must be available (in-memory or loadable from storage).
 * @post No modification to inventory is performed by a lookup; the selected
 *       index may be used by edit/delete operations.
 */
int lookUpBook();

/**
 * @brief Add a new book to inventory (interactive).
 * @pre The program must have write access to persistence storage if changes
 *      are saved immediately.
 * @post A new book record will be added to the inventory if input is valid.
 */
void addBook();

/**
 * @brief Edit an existing book's information (interactive).
 * @pre A target book must have been selected (typically via `lookUpBook`).
 * @post The chosen book's fields may be updated; invariants must be
 *       preserved (e.g., non-negative quantity/price).
 * @todo Consider adding a version of `editBook` that accepts an index/ID
 *       parameter to allow non-interactive edits.
 */
void editBook();

/**
 * @brief Delete a book from inventory (interactive confirmation required).
 * @pre A target book must have been selected (typically via `lookUpBook`).
 * @post If confirmed, the book will be removed from inventory and
 *       persistence storage may be updated.
 */
void deleteBook();

#endif // INV_MENU_H
