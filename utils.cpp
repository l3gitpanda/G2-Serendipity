#include <sstream>
#include <cctype>
#include <limits>
#include <iostream>
#include <string>

using namespace std;

std::string trim(const std::string &value)
{
  size_t start = 0;
  while (start < value.size() && isspace(static_cast<unsigned char>(value[start])))
  {
    ++start;
  }

  if (start == value.size())
  {
    return "";
  }

  size_t end = value.size();
  while (end > start && isspace(static_cast<unsigned char>(value[end - 1])))
  {
    --end;
  }

  return value.substr(start, end - start);
}

bool parseNonNegativeInt(const std::string &text, int &value)
{
  long long result = 0;
  for (char ch : text)
  {
    if (!isdigit(static_cast<unsigned char>(ch)))
    {
      return false;
    }

    result = result * 10 + (ch - '0');

    if (result > numeric_limits<int>::max())
    {
      return false;
    }
  }

  value = static_cast<int>(result);
  return true;
}

bool parseNonNegativeDouble(const std::string &text, double &value)
{
  istringstream iss(text);
  double parsed = 0.0;
  iss >> parsed;

  if (!iss || parsed < 0.0)
  {
    return false;
  }

  char leftover;
  if (iss >> leftover)
  {
    return false;
  }

  value = parsed;
  return true;
}

bool promptStringField(const std::string &label, std::string &out)
{
  while (true)
  {
    cout << label << ": ";
    string input;
    getline(cin, input);

    if (input.empty())
    {
      cout << "Add book cancelled.\n";
      return false;
    }

    string trimmed = trim(input);
    if (trimmed.empty())
    {
      cout << label << " cannot be blank.\n";
      continue;
    }

    out = trimmed;
    return true;
  }
}

bool promptNonNegativeIntField(const std::string &label, int &out)
{
  while (true)
  {
    cout << label << ": ";
    string input;
    getline(cin, input);

    if (input.empty())
    {
      cout << "Add book cancelled.\n";
      return false;
    }

    string trimmed = trim(input);
    if (trimmed.empty())
    {
      cout << "Please enter a non-negative integer for " << label << ".\n";
      continue;
    }

    int value = 0;
    if (!parseNonNegativeInt(trimmed, value))
    {
      cout << "Please enter a non-negative integer for " << label << ".\n";
      continue;
    }

    out = value;
    return true;
  }
}

bool promptNonNegativeDoubleField(const std::string &label, double &out)
{
  while (true)
  {
    cout << label << ": ";
    string input;
    getline(cin, input);

    if (input.empty())
    {
      cout << "Add book cancelled.\n";
      return false;
    }

    string trimmed = trim(input);
    if (trimmed.empty())
    {
      cout << "Please enter a non-negative number for " << label << ".\n";
      continue;
    }

    double value = 0.0;
    if (!parseNonNegativeDouble(trimmed, value))
    {
      cout << "Please enter a non-negative number for " << label << ".\n";
      continue;
    }

    out = value;
    return true;
  }
}
/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 9/23/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp -o serendipity.out
*/

#include <iostream>
#include "utils.h"
#include <limits>

using namespace std;

void clearScreen() {
    // Clear the screen
    cout << "\033[2J\033[1;1H" << endl;
}

void pressEnterToContinue() {
    cout << endl << "Press enter to continue..." << flush;
    cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
}

