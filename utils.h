/*
CS1B – Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 10/14/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/

#ifndef UTILS_H
#define UTILS_H
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

void clearScreen();
void pressEnterToContinue();

struct navigationMenu
{
    std::string heading;
    std::vector<std::string> options;

    void print(int optionNumberPadding, int optionLabelMargin) const
    {
        std::cout << std::right << std::setw(40) << "SERENDIPITY" << std::endl
                  << std::right << std::setw(50) << heading << std::endl;

        for (std::size_t i = 0; i < options.size(); ++i)
        {
            std::cout << std::right << std::setw(optionNumberPadding) << (i + 1) << "."
                      << std::right << std::setw(optionLabelMargin) << ""
                      << std::left << options[i] << std::endl;
        }
    }
};

std::string trim(const std::string &value);
bool parseNonNegativeInt(const std::string &text, int &value);
bool parseNonNegativeDouble(const std::string &text, double &value);
bool isValidDate(const std::string &value);
std::string toLowerCopy(const std::string &value);
std::string formatMoney(double value);

#endif // UTILS_H