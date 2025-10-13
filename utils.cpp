#include "utils.h"

#include <cctype>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

std::string trim(const std::string &value)
{
    std::size_t start = 0;
    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start])))
    {
        ++start;
    }

    if (start == value.size())
    {
        return "";
    }

    std::size_t end = value.size();
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1])))
    {
        --end;
    }

    return value.substr(start, end - start);
}

bool parseNonNegativeInt(const std::string &text, int &value)
{
    std::string trimmed = trim(text);
    if (trimmed.empty())
    {
        return false;
    }

    long long result = 0;
    for (char ch : trimmed)
    {
        if (!std::isdigit(static_cast<unsigned char>(ch)))
        {
            return false;
        }

        result = result * 10 + (ch - '0');
        if (result > std::numeric_limits<int>::max())
        {
            return false;
        }
    }

    value = static_cast<int>(result);
    return true;
}

bool parseNonNegativeDouble(const std::string &text, double &value)
{
    std::string trimmed = trim(text);
    if (trimmed.empty())
    {
        return false;
    }

    std::istringstream input(trimmed);
    double parsed = 0.0;
    input >> parsed;
    if (!input || parsed < 0.0)
    {
        return false;
    }

    char leftover;
    if (input >> leftover)
    {
        return false;
    }

    value = parsed;
    return true;
}

bool isValidDate(const std::string &value)
{
    if (value.size() != 10)
    {
        return false;
    }

    if (value[2] != '/' || value[5] != '/')
    {
        return false;
    }

    for (std::size_t i = 0; i < value.size(); ++i)
    {
        if (i == 2 || i == 5)
        {
            continue;
        }

        if (!std::isdigit(static_cast<unsigned char>(value[i])))
        {
            return false;
        }
    }

    int month = std::stoi(value.substr(0, 2));
    int day   = std::stoi(value.substr(3, 2));
    int year  = std::stoi(value.substr(6, 4));

    if (month < 1 || month > 12)
    {
        return false;
    }

    if (day < 1 || day > 31)
    {
        return false;
    }

    if (year < 0)
    {
        return false;
    }

    return true;
}

std::string toLowerCopy(const std::string &value)
{
    std::string result = value;
    for (char &ch : result)
    {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return result;
}

std::string formatMoney(double value)
{
    std::ostringstream output;
    output.setf(std::ios::fixed);
    output.precision(2);
    output << "$" << value;
    return output.str();
}

void clearScreen()
{
    std::cout << "\033[2J\033[H";
    std::cout.flush();
}

void pressEnterToContinue()
{
    std::cout << "\nPress Enter to continue..." << std::flush;
    if (!std::cin.good())
    {
        std::cin.clear();
    }

    if (std::cin.peek() == '\n')
    {
        std::cin.get();
    }
    else
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
