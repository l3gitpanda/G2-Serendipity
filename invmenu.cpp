/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 9/23/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp -o serendipity.out
*/

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>
#include <cctype>
#include "invmenu.h"
#include "utils.h"
#include "bookType.h"
using namespace std;

namespace
{
  static vector<BookType> inventory;
  constexpr size_t kMaxInventory = 20;

  string trim(const string &value)
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

  bool parseNonNegativeInt(const string &text, int &value)
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

  bool parseNonNegativeDouble(const string &text, double &value)
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

  bool promptStringField(const string &label, string &out)
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

  bool promptNonNegativeIntField(const string &label, int &out)
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

  bool promptNonNegativeDoubleField(const string &label, double &out)
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

  bool confirmSave()
  {
    while (true)
    {
      cout << "Save this record? (Y/N): ";
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
        cout << "Please enter Y or N.\n";
        continue;
      }

      char response = static_cast<char>(tolower(static_cast<unsigned char>(trimmed[0])));
      if (response == 'y')
      {
        return true;
      }

      if (response == 'n')
      {
        cout << "Add book cancelled.\n";
        return false;
      }

      cout << "Please enter Y or N.\n";
    }
  }
} // namespace

static void printInvMenu()
{
  cout << "Serendipity Booksellers\n\n";
  cout << "Inventory Database\n\n";
  cout << "1. Look Up a Book\n";
  cout << "2. Add a Book\n";
  cout << "3. Edit a Book's Record\n";
  cout << "4. Delete a Book\n";
  cout << "5. Return to the Main Menu\n\n";
  cout << "Enter Your Choice: ";
}

void invMenu()
{
  bool running = true;

  while (running)
  {
    clearScreen();
    printInvMenu();

    string input;
    getline(cin, input);

    if (input.size() != 1 || input[0] < '1' || input[0] > '5')
    {
      cout << "\nPlease enter a number in the range 1 – 5.\n";
      pressEnterToContinue();
      continue;
    }

    switch (input[0])
    {
      case '1':
        clearScreen();
        lookUpBook();
        pressEnterToContinue();
        break;
      case '2':
        clearScreen();
        addBook();
        pressEnterToContinue();
        break;
      case '3':
        clearScreen();
        editBook();
        pressEnterToContinue();
        break;
      case '4':
        clearScreen();
        deleteBook();
        pressEnterToContinue();
        break;
      case '5':
        running = false; // Return to Main Menu
        break;
    }
  }
}

// Inventory stubs (navigation-only)
void lookUpBook()  { cout << "You selected Look Up Book.\n"; }
void addBook()
{
  if (inventory.size() >= kMaxInventory)
  {
    cout << "Inventory full (20/20). Cannot add more books.\n";
    return;
  }

  cout << "Add a Book to Inventory\n";
  cout << "-----------------------\n";

  BookType record;

  string bookTitle, isbn, author, publisher, dateAdded;
  int qtyOnHand;
  double wholesale, retail;

  if (!promptStringField("Book Title", bookTitle))
    return;
  if (!promptStringField("ISBN", isbn))
    return;
  if (!promptStringField("Author", author))
    return;
  if (!promptStringField("Publisher", publisher))
    return;
  if (!promptStringField("Date Added", dateAdded))
    return;
  if (!promptNonNegativeIntField("Quantity on Hand", qtyOnHand))
    return;
  if (!promptNonNegativeDoubleField("Wholesale Cost", wholesale))
    return;
  if (!promptNonNegativeDoubleField("Retail Price", retail))
    return;

  if (retail < wholesale)
    cout << "Warning: Retail price is less than wholesale cost.\n";

  if (!confirmSave())
    return;

  BookType record;
  record.setBookTitle(bookTitle);
  record.setIsbn(isbn);
  record.setAuthor(author);
  record.setPublisher(publisher);
  record.setDateAdded(dateAdded);
  record.setQtyOnHand(qtyOnHand);
  record.setWholesale(wholesale);
  record.setRetail(retail);

  inventory.push_back(record);

  cout << "\nBook added successfully.\n";
  cout << "Inventory count: " << inventory.size() << '/' << kMaxInventory << ".\n";
  if (inventory.size() == kMaxInventory)
    cout << "Inventory now full (20/20).\n";
}
void editBook()    { cout << "You selected Edit Book.\n"; }
void deleteBook()  { cout << "You selected Delete Book.\n"; }