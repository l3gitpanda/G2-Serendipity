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
};

std::string trim(const std::string &value);
bool parseNonNegativeInt(const std::string &text, int &value);
bool parseNonNegativeDouble(const std::string &text, double &value);
bool isValidDate(const std::string &value);
std::string toLowerCopy(const std::string &value);
std::string formatMoney(double value);

#endif // UTILS_H