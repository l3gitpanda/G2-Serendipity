/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 10/10/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp -o serendipity.out
*/

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "bookType.h"
#include "bookinfo.h"
#include "invmenu.h"
#include "utils.h"

using namespace std;

namespace
{
  constexpr size_t kMaxInventory = 20;

  // Container choice: std::vector<BookType> with capacity capped at 20 records.
  static vector<BookType> inventory;

  struct PendingBook
  {
    string title;
    string isbn;
    string author;
    string publisher;
    string dateAdded;
    int quantity = 0;
    double wholesale = 0.0;
    double retail = 0.0;

    bool titleSet = false;
    bool isbnSet = false;
    bool authorSet = false;
    bool publisherSet = false;
    bool dateSet = false;
    bool qtySet = false;
    bool wholesaleSet = false;
    bool retailSet = false;
  };

  string toLowerCopy(const string &value)
  {
    string lowered;
    lowered.reserve(value.size());
    for (char ch : value)
    {
      lowered.push_back(static_cast<char>(tolower(static_cast<unsigned char>(ch))));
    }
    return lowered;
  }

  bool isValidDate(const string &date)
  {
    if (date.size() != 10)
    {
      return false;
    }

    for (size_t i = 0; i < date.size(); ++i)
    {
      if (i == 2 || i == 5)
      {
        if (date[i] != '/')
        {
          return false;
        }
        continue;
      }

      if (!isdigit(static_cast<unsigned char>(date[i])))
      {
        return false;
      }
    }

    const int month = stoi(date.substr(0, 2));
    const int day = stoi(date.substr(3, 2));

    if (month < 1 || month > 12)
    {
      return false;
    }

    if (day < 1 || day > 31)
    {
      return false;
    }

    return true;
  }

  bool allFieldsFilled(const PendingBook &pending)
  {
    return pending.titleSet && pending.isbnSet && pending.authorSet && pending.publisherSet &&
           pending.dateSet && pending.qtySet && pending.wholesaleSet && pending.retailSet;
  }

  void renderInventoryHeader(const string &subtitle)
  {
    clearScreen();
    cout << "Serendipity Booksellers\n";
    cout << subtitle << "\n\n";
    cout << "DATABASE SIZE: " << kMaxInventory << '\n';
    cout << "CURRENT BOOK COUNT: " << BookType::getBookCount() << "\n\n";
  }

  string formatMoney(double amount)
  {
    ostringstream out;
    out << fixed << setprecision(2) << '$' << amount;
    return out.str();
  }

  void displayPendingBook(const PendingBook &pending)
  {
    cout << left << setw(28) << "1. Enter Book Title" << ": "
         << (pending.titleSet ? pending.title : "EMPTY") << '\n';
    cout << left << setw(28) << "2. Enter ISBN" << ": "
         << (pending.isbnSet ? pending.isbn : "EMPTY") << '\n';
    cout << left << setw(28) << "3. Enter Author" << ": "
         << (pending.authorSet ? pending.author : "EMPTY") << '\n';
    cout << left << setw(28) << "4. Enter Publisher" << ": "
         << (pending.publisherSet ? pending.publisher : "EMPTY") << '\n';
    cout << left << setw(28) << "5. Enter Date Added" << ": "
         << (pending.dateSet ? pending.dateAdded : "EMPTY") << '\n';
    cout << left << setw(28) << "6. Enter Quantity on Hand" << ": "
         << (pending.qtySet ? to_string(pending.quantity) : "0") << '\n';
    cout << left << setw(28) << "7. Enter Wholesale Cost" << ": "
         << (pending.wholesaleSet ? formatMoney(pending.wholesale) : "$0.00") << '\n';
    cout << left << setw(28) << "8. Enter Retail Price" << ": "
         << (pending.retailSet ? formatMoney(pending.retail) : "$0.00") << '\n';
    cout << "9. Save Book to Database\n";
    cout << "10. Return to Inventory Menu\n\n";
  }

  void promptForString(const string &label, string &out, bool &flag)
  {
    while (true)
    {
      cout << label << ": ";
      string input;
      getline(cin, input);
      string trimmed = trim(input);
      if (trimmed.empty())
      {
        cout << label << " cannot be blank.\n";
        continue;
      }

      out = trimmed;
      flag = true;
      break;
    }
  }

  void promptForDate(string &out, bool &flag)
  {
    while (true)
    {
      cout << "Date Added (mm/dd/yyyy): ";
      string input;
      getline(cin, input);
      string trimmed = trim(input);

      if (trimmed.empty())
      {
        cout << "Date Added cannot be blank.\n";
        continue;
      }

      if (!isValidDate(trimmed))
      {
        cout << "Please enter a valid date in mm/dd/yyyy format.\n";
        continue;
      }

      out = trimmed;
      flag = true;
      break;
    }
  }

  void promptForNonNegativeInt(const string &label, int &out, bool &flag)
  {
    while (true)
    {
      cout << label << ": ";
      string input;
      getline(cin, input);
      string trimmed = trim(input);
      if (trimmed.empty())
      {
        cout << label << " must be provided.\n";
        continue;
      }

      int value = 0;
      if (!parseNonNegativeInt(trimmed, value))
      {
        cout << label << " must be a non-negative integer.\n";
        continue;
      }

      out = value;
      flag = true;
      break;
    }
  }

  void promptForNonNegativeDouble(const string &label, double &out, bool &flag)
  {
    while (true)
    {
      cout << label << ": ";
      string input;
      getline(cin, input);
      string trimmed = trim(input);
      if (trimmed.empty())
      {
        cout << label << " must be provided.\n";
        continue;
      }

      double value = 0.0;
      if (!parseNonNegativeDouble(trimmed, value))
      {
        cout << label << " must be a non-negative number.\n";
        continue;
      }

      out = value;
      flag = true;
      break;
    }
  }

  vector<size_t> searchByTitle(const string &needle)
  {
    vector<size_t> matches;
    const string loweredNeedle = toLowerCopy(needle);

    for (size_t index = 0; index < inventory.size(); ++index)
    {
      const string loweredTitle = toLowerCopy(inventory[index].getBookTitle());
      if (loweredTitle.find(loweredNeedle) != string::npos)
      {
        matches.push_back(index);
      }
    }

    return matches;
  }

  vector<size_t> searchByIsbn(const string &needle)
  {
    vector<size_t> matches;
    const string loweredNeedle = toLowerCopy(needle);

    for (size_t index = 0; index < inventory.size(); ++index)
    {
      const string loweredIsbn = toLowerCopy(inventory[index].getIsbn());
      if (loweredIsbn.find(loweredNeedle) != string::npos)
      {
        matches.push_back(index);
      }
    }

    return matches;
  }

  void displaySearchResults(const vector<size_t> &matches)
  {
    cout << "\nSearch Results\n";
    cout << "--------------\n";

    for (size_t i = 0; i < matches.size(); ++i)
    {
      const BookType &book = inventory[matches[i]];
      cout << (i + 1) << ". " << book.getBookTitle() << " (ISBN: " << book.getIsbn() << ")\n";
    }

    cout << (matches.size() + 1) << ". Return to Search Menu\n\n";
  }

  bool confirmSave()
  {
    while (true)
    {
      cout << "Save this record? (Y/N): ";
      string input;
      getline(cin, input);

      string trimmed = trim(input);
      if (trimmed.empty())
      {
        cout << "Please enter Y or N.\n";
        continue;
      }

      const char response = static_cast<char>(tolower(static_cast<unsigned char>(trimmed[0])));
      if (response == 'y')
      {
        return true;
      }
      if (response == 'n')
      {
        cout << "Save cancelled.\n";
        return false;
      }

      cout << "Please enter Y or N.\n";
    }
  }
}

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
        lookUpBook();
        break;
      case '2':
        addBook();
        break;
      case '3':
        clearScreen();
        cout << "You selected Edit Book.\n";
        pressEnterToContinue();
        break;
      case '4':
        clearScreen();
        cout << "You selected Delete Book.\n";
        pressEnterToContinue();
        break;
      case '5':
        running = false; // Return to Main Menu
        break;
    }
  }
}

void lookUpBook()
{
  bool returnToInventory = false;

  while (!returnToInventory)
  {
    renderInventoryHeader("Inventory Database: Look Up Book");

    cout << "1. Search by Title\n";
    cout << "2. Search by ISBN\n";
    cout << "3. Return to Inventory Menu\n\n";
    cout << "Enter Your Choice: ";

    string choice;
    getline(cin, choice);

    if (choice.size() != 1 || choice[0] < '1' || choice[0] > '3')
    {
      cout << "\nPlease enter a number in the range 1 – 3.\n";
      pressEnterToContinue();
      continue;
    }

    if (choice[0] == '3')
    {
      returnToInventory = true;
      continue;
    }

    cout << ((choice[0] == '1') ? "Enter title to search for: " : "Enter ISBN to search for: ");
    string searchValue;
    getline(cin, searchValue);
    searchValue = trim(searchValue);

    if (searchValue.empty())
    {
      cout << "\nPlease enter a search term.\n";
      pressEnterToContinue();
      continue;
    }

    vector<size_t> matches = (choice[0] == '1') ? searchByTitle(searchValue) : searchByIsbn(searchValue);

    if (matches.empty())
    {
      cout << "\nNot in inventory.\n";
      pressEnterToContinue();
      continue;
    }

    while (true)
    {
      renderInventoryHeader("Inventory Database: Look Up Book");
      cout << "Search term: " << searchValue << "\n";
      displaySearchResults(matches);
      cout << "Enter Your Choice: ";

      string resultChoice;
      getline(cin, resultChoice);

      if (resultChoice.empty() || resultChoice.find_first_not_of("0123456789") != string::npos)
      {
        cout << "\nPlease enter a valid option.\n";
        pressEnterToContinue();
        continue;
      }

      size_t selection = static_cast<size_t>(stoi(resultChoice));
      if (selection == 0 || selection > matches.size() + 1)
      {
        cout << "\nPlease enter a valid option.\n";
        pressEnterToContinue();
        continue;
      }

      if (selection == matches.size() + 1)
      {
        break; // return to previous menu
      }

      const BookType &chosen = inventory[matches[selection - 1]];
      renderInventoryHeader("Inventory Database: Book Info");
      bookInfo(chosen);
      pressEnterToContinue();
    }
  }
}

void addBook()
{
  if (inventory.size() >= kMaxInventory)
  {
    clearScreen();
    cout << "Inventory full (20/20). Cannot add more books.\n";
    pressEnterToContinue();
    return;
  }

  PendingBook pending;
  bool editing = true;

  while (editing)
  {
    renderInventoryHeader("Inventory Database: Add Book");
    displayPendingBook(pending);
    cout << "Enter Your Choice: ";

    string choice;
    getline(cin, choice);

    if (choice.empty())
    {
      cout << "\nPlease enter a valid option.\n";
      pressEnterToContinue();
      continue;
    }

    if (choice == "1")
    {
      promptForString("Enter Book Title", pending.title, pending.titleSet);
    }
    else if (choice == "2")
    {
      promptForString("Enter ISBN", pending.isbn, pending.isbnSet);
    }
    else if (choice == "3")
    {
      promptForString("Enter Author", pending.author, pending.authorSet);
    }
    else if (choice == "4")
    {
      promptForString("Enter Publisher", pending.publisher, pending.publisherSet);
    }
    else if (choice == "5")
    {
      promptForDate(pending.dateAdded, pending.dateSet);
    }
    else if (choice == "6")
    {
      promptForNonNegativeInt("Enter Quantity on Hand", pending.quantity, pending.qtySet);
    }
    else if (choice == "7")
    {
      promptForNonNegativeDouble("Enter Wholesale Cost", pending.wholesale, pending.wholesaleSet);
    }
    else if (choice == "8")
    {
      promptForNonNegativeDouble("Enter Retail Price", pending.retail, pending.retailSet);
    }
    else if (choice == "9")
    {
      if (inventory.size() >= kMaxInventory)
      {
        cout << "\nInventory full (20/20). Cannot add more books.\n";
        pressEnterToContinue();
        return;
      }

      if (!allFieldsFilled(pending))
      {
        cout << "\nAll fields must be completed before saving.\n";
        pressEnterToContinue();
        continue;
      }

      if (pending.retail < pending.wholesale)
      {
        cout << "\nRetail price cannot be less than wholesale cost.\n";
        pressEnterToContinue();
        continue;
      }

      if (!confirmSave())
      {
        pressEnterToContinue();
        continue;
      }

      inventory.emplace_back(pending.title, pending.isbn, pending.author, pending.publisher,
                             pending.dateAdded, pending.quantity, pending.wholesale, pending.retail);

      cout << "\nBook added successfully.\n";
      cout << "Inventory count: " << inventory.size() << '/' << kMaxInventory << "\n";
      if (inventory.size() == kMaxInventory)
      {
        cout << "Inventory now full (20/20).\n";
      }
      pressEnterToContinue();
      editing = false;
    }
    else if (choice == "10")
    {
      cout << "\nReturning to Inventory Menu.\n";
      pressEnterToContinue();
      editing = false;
    }
    else
    {
      cout << "\nPlease enter a valid option.\n";
      pressEnterToContinue();
    }
  }
}

void editBook()
{
  clearScreen();
  cout << "You selected Edit Book.\n";
  pressEnterToContinue();
}

void deleteBook()
{
  clearScreen();
  cout << "You selected Delete Book.\n";
  pressEnterToContinue();
}
