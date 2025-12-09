/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 11/16/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/


/**
 * @file reports.h
 * @brief Reporting utilities: generate various inventory reports.
 *
 * @section uml_reports Reports UML Overview
 * - Stereotype: «boundary» / «control»
 * - Responsibilities: offer a menu of inventory reports and render
 *   summaries such as listings, wholesale totals, and age-based views.
 * - Collaborators: reads from inventory data structures and uses
 *   formatting helpers for monetary values and dates.
 */

#ifndef REPORTS_H
#define REPORTS_H

#include <cstddef>
#include <vector>

#include "bookType.h"

/**
 * @brief Entry point for the reports menu.
 * @post Control returns to the caller after the selected report is displayed.
 */
void reports();

/**
 * @brief Print a full listing of books in inventory.
 * @pre Inventory should be loaded.
 * @post The listing is printed to stdout or configured output.
 */
void repListing();

/**
 * @brief Render the inventory listing for a vector-backed inventory.
 * @param books Collection of book pointers to render; ownership stays with the
 *              caller and each entry must be non-null.
 * @param capacity Logical capacity of the backing storage, displayed in the
 *                  header alongside the live record count.
 *
 * Displays tabular columns for title, ISBN, author, publisher, date added,
 * quantity, wholesale, and retail. Each page shows "REPORT LISTING" with the
 * current date, page X of Y, and both the configured database size and the
 * live `bookType::recordCount()`. Output is paginated at ten rows per page; if
 * more data remains, the function prompts for a key press before continuing to
 * the next page.
 */
void reportListing(const std::vector<bookType *> &books, std::size_t capacity);

/**
 * @brief Render the inventory listing for a raw array of book pointers.
 * @param books Raw pointer array containing `count` entries. Pointers are
 *              treated as borrowed and must point to valid `bookType`
 *              instances.
 * @param count Number of valid book pointers stored in @p books.
 * @param capacity Logical capacity of the backing storage, displayed in the
 *                  header alongside the live record count.
 *
 * Provides the same paginated, ten-rows-per-page output, page header, and
 * continuation prompt as the vector-based overload.
 */
void reportListing(bookType *const *books, std::size_t count, std::size_t capacity);

/**
 * @brief Print a wholesale-cost report.
 * @pre Inventory should be loaded.
 * @post Wholesale cost information printed, suitable for accounting.
 */
void repWholesale();

/**
 * @brief Print a retail price report.
 * @pre Inventory should be loaded.
 * @post Retail report printed.
 */
void repRetail();

/**
 * @brief Report books by quantity on hand.
 * @pre Inventory should be loaded.
 * @post Quantities printed or summarized.
 */
void repQty();

/**
 * @brief Report the total cost/value of inventory.
 * @pre Inventory should be loaded.
 * @post Cost/valuation printed.
 */
void repCost();

/**
 * @brief Generate an age-based inventory report (by date added).
 * @pre Inventory should be loaded and `dateAdded` should be parseable.
 * @post Age-sorted or grouped report printed.
 */
void repAge();

#endif
