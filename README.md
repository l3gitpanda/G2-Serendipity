/*!
 * @mainpage G2-Serendipity
 *
 * @section overview Overview
 * Serendipity is a simple menu-driven inventory management system for books
 * used as a CS1B group project. It supports adding, editing, deleting,
 * looking up books, generating reports and a cashier workflow.
 *
 * @section data_model Inventory Data Model
 * Serendipity keeps inventory in a pointer-backed container (`std::vector<bookType*>`).
 * The inventory module owns these pointers outright: it allocates with `new` when a
 * book is added and deletes each pointer when clearing the database, removing a record,
 * or shutting down. No shared ownership is used—callers must never `delete` inventory
 * pointers they did not allocate.
 *
 * The `bookType` class tracks the number of live records through the static
 * `bookType::num_recs` counter. Every constructor (including copy/move) increments the
 * counter when a new object is created, and the destructor decrements it when a record
 * is destroyed. Calls to `bookType::recordCount()` return the current number of live
 * records and are used by reports to show how many books exist.
 *
 * @section reports Reporting & Pagination
 * The **Report Listing** view paginates output ten rows per page and prints a header on
 * each page that includes the store name, "REPORT LISTING", the current date, and page
 * X of Y metadata. The header also echoes the configured database capacity alongside the
 * live `bookType::recordCount()` value so the pointer-backed ownership model is visible
 * during reports.
 *
 * @section usage Usage
 * Build the project and run `serendipity.out` to launch the console UI.
 * See the `Build/Run` section for compilation instructions.
 *
 * @section contracts Design Contracts
 * - Functions generally validate user input and preserve invariants such as
 *   non-negative quantities and prices.
 * - Interactive functions (add/edit/delete) require user confirmation for
 *   destructive actions.
 *
 * @section build Build/Run
 * Compile with the provided build command from the project header comments:
 *
 * - `g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out`
 *
 * Run the program with `./serendipity.out`.
 *
 * @section team Team
 * - Ryan Posey
 * - Ian Bagherzadeh
 *
 * @note Doxygen-generated HTML is available after running `doxygen Doxyfile`.
 */

# G2-Serendipity
## Colaborators
- Ryan Posey
- Ian Bagherzadeh
