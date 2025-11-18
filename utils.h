/*
CS1B – Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 11/16/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/

/**
 * @file utils.h
 * @brief Utility helpers used across the Serendipity application.
 */

#ifndef UTILS_H
#define UTILS_H
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Clear the terminal screen (best-effort).
 * @post The terminal screen will be cleared if supported by the environment.
 */
void clearScreen();

/**
 * @brief Pause execution until the user presses Enter.
 * @post Returns after a newline is read from stdin.
 */
void pressEnterToContinue();

/**
 * @brief Simple framed navigation menu helper used for console UI.
 *
 * Example usage:
 * @code
 * navigationMenu m{"Main Menu", {"Option A","Option B"}};
 * m.print();
 * @endcode
 */
struct navigationMenu
{
    std::string heading;
    std::vector<std::string> options;

    /**
     * @brief Print the menu with a fixed-width frame.
     * @note Uses `std::cout` directly.
     */
    void print() const
    {
        constexpr int FRAME_W = 80;
        constexpr int INSIDE = 78;
        auto border = []() { std::cout << std::string(FRAME_W, '-') << '\n'; };
        auto framed = [](const std::string& s) {
            std::string t = s;
            if ((int)t.size() < INSIDE) t += std::string(INSIDE - (int)t.size(), ' ');
            else if ((int)t.size() > INSIDE) t = t.substr(0, INSIDE);
            std::cout << '-' << t << "-\n";
        };
        auto blank = [&]() { framed(""); };

        border();
        framed("SERENDIPITY");
        blank();
        framed(heading);
        blank();

        for (std::size_t i = 0; i < options.size(); ++i)
        {
            std::string opt = std::to_string(i + 1) + ". " + options[i];
            framed(opt);
        }
        border();
    }

    /**
     * @brief Print the menu and include a "Return" line as the highest number.
     * @param backLabel Label text for the return option.
     */
    void printWithBack(const std::string &backLabel = "Return to Previous Menu") const
    {
        constexpr int FRAME_W = 80;
        constexpr int INSIDE = 78;
        auto border = []() { std::cout << std::string(FRAME_W, '-') << '\n'; };
        auto framed = [](const std::string& s) {
            std::string t = s;
            if ((int)t.size() < INSIDE) t += std::string(INSIDE - (int)t.size(), ' ');
            else if ((int)t.size() > INSIDE) t = t.substr(0, INSIDE);
            std::cout << '-' << t << "-\n";
        };
        auto blank = [&]() { framed(""); };

        border();
        framed("SERENDIPITY");
        blank();
        framed(heading);
        blank();

        for (std::size_t i = 0; i < options.size(); ++i)
        {
            std::string opt = std::to_string(i + 1) + ". " + options[i];
            framed(opt);
        }

        // Return option is printed as the last (highest) number
        int returnIndex = static_cast<int>(options.size()) + 1;
        std::string ret = std::to_string(returnIndex) + ". " + backLabel;
        framed(ret);
        border();
    }
};

/**
 * @brief Trim leading/trailing whitespace from a string.
 * @param value Input string to trim.
 * @return New string with leading and trailing whitespace removed.
 */
std::string trim(const std::string &value);

/**
 * @brief Parse a non-negative integer from text.
 * @param text Text to parse.
 * @param value Output reference set on success.
 * @return true if parsing succeeded and `value >= 0`.
 */
bool parseNonNegativeInt(const std::string &text, int &value);

/**
 * @brief Parse a non-negative double from text.
 * @param text Text to parse.
 * @param value Output reference set on success.
 * @return true if parsing succeeded and `value >= 0.0`.
 */
bool parseNonNegativeDouble(const std::string &text, double &value);

/**
 * @brief Rudimentary date validation for the project's date format.
 * @param value Date string to validate.
 * @return true if the value looks like a valid date for the project.
 */
bool isValidDate(const std::string &value);

/**
 * @brief Return a lower-cased copy of the input string.
 * @param value Input string.
 * @return Lower-cased copy of `value`.
 */
std::string toLowerCopy(const std::string &value);

/**
 * @brief Format a monetary value to a string with two decimal places.
 * @param value Numeric monetary amount.
 * @return Formatted money string (e.g., "12.34").
 */
std::string formatMoney(double value);

#endif // UTILS_H