/*!
 * @mainpage G2-Serendipity
 *
 * @section overview Overview
 * Serendipity is a simple menu-driven inventory management system for books
 * used as a CS1B group project. It supports adding, editing, deleting,
 * looking up books, generating reports and a cashier workflow.
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
