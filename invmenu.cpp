/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 10/10/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "invmenu.h"
#include "utils.h"
#include "bookType.h"
#include "bookinfo.h"

using namespace std;

namespace
{
  // Container choice: using std::vector<BookType> with a fixed logical capacity of 20 records.
  static vector<BookType> inventory;
  constexpr size_t kMaxInventory = 20;

  string toLowerCase(const string &value)
  {
    string lower;
    lower.reserve(value.size());
    for (char ch : value)
    {
      lower.push_back(static_cast<char>(tolower(static_cast<unsigned char>(ch))));
    }
    return lower;
  }

  bool isValidDate(const string &value)
  {
    if (value.size() != 10)
    {
      return false;
    }

    if (value[2] != '/' || value[5] != '/')
    {
      return false;
    }

    for (size_t i = 0; i < value.size(); ++i)
    {
      if (i == 2 || i == 5)
      {
        continue;
      }

      if (!isdigit(static_cast<unsigned char>(value[i])))
      {
        return false;
      }
    }

    int month = stoi(value.substr(0, 2));
    int day = stoi(value.substr(3, 2));
    int year = stoi(value.substr(6, 4));

    if (month < 1 || month > 12)
    {
      return false;
    }

    if (day < 1 || day > 31)
    {
      return false;
    }

    if (year < 1000 || year > 9999)
    {
      return false;
    }

    return true;
  }

  string formatStringField(const string &value, bool hasValue)
  {
    return hasValue ? value : "EMPTY";
  }

  string formatIntField(int value, bool hasValue)
  {
    if (!hasValue)
    {
      return "EMPTY";
    }

    return to_string(value);
  }

  string formatMoneyField(double value, bool hasValue)
  {
    if (!hasValue)
    {
      return "EMPTY";
    }

    ostringstream oss;
    oss << fixed << setprecision(2) << '$' << value;
    return oss.str();
  }

  vector<size_t> searchByTitle(const string &searchTerm)
  {
    vector<size_t> matches;
    string needle = toLowerCase(searchTerm);

    for (size_t index = 0; index < inventory.size(); ++index)
    {
      string title = toLowerCase(inventory[index].getBookTitle());
      if (title.find(needle) != string::npos)
      {
        matches.push_back(index);
      }
    }

    return matches;
  }

  vector<size_t> searchByIsbn(const string &searchTerm)
  {
    vector<size_t> matches;
    string needle = toLowerCase(searchTerm);

    for (size_t index = 0; index < inventory.size(); ++index)
    {
      string isbn = toLowerCase(inventory[index].getIsbn());
      if (isbn.find(needle) != string::npos)
      {
        matches.push_back(index);
      }
    }

    return matches;
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

      char response = static_cast<char>(tolower(static_cast<unsigned char>(trimmed[0])));
      if (response == 'y')
      {
        return true;
      }

      if (response == 'n')
      {
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

void lookUpBook()
{
  bool exitMenu = false;

  while (!exitMenu)
  {
    clearScreen();

    cout << "Serendipity Booksellers\n\n";
    cout << "Look Up Book\n\n";
    cout << "DATABASE SIZE: " << kMaxInventory
         << "    CURRENT BOOK COUNT: " << BookType::getBookCount() << "\n\n";
    cout << "1. Search by Title\n";
    cout << "2. Search by ISBN\n";
    cout << "3. Return to Inventory Menu\n\n";
    cout << "Enter Your Choice: ";

    string rawChoice;
    getline(cin, rawChoice);
    string trimmedChoice = trim(rawChoice);

    int option = 0;
    if (!trimmedChoice.empty())
    {
      if (!parseNonNegativeInt(trimmedChoice, option))
      {
        option = 0;
      }
    }

    if (option < 1 || option > 3)
    {
      cout << "\nPlease enter a number in the range 1 – 3.\n";
      pressEnterToContinue();
      continue;
    }

    if (option == 3)
    {
      cout << "\nReturning to Inventory Menu.\n";
      break;
    }

    string prompt = option == 1 ? "Enter title to search for: " : "Enter ISBN to search for: ";
    string searchTerm;
    do
    {
      cout << prompt;
      getline(cin, searchTerm);
      searchTerm = trim(searchTerm);

      if (searchTerm.empty())
      {
        cout << "Search text cannot be blank.\n";
      }
    } while (searchTerm.empty());

    vector<size_t> matches = option == 1 ? searchByTitle(searchTerm) : searchByIsbn(searchTerm);

    if (matches.empty())
    {
      cout << "\n\"" << searchTerm << "\" not in inventory.\n";
      pressEnterToContinue();
      continue;
    }

    bool returnToSearchMenu = false;
    while (!returnToSearchMenu)
    {
      clearScreen();
      cout << "Serendipity Booksellers\n\n";
      cout << "Search Results\n\n";

      for (size_t i = 0; i < matches.size(); ++i)
      {
        const BookType &book = inventory[matches[i]];
        cout << (i + 1) << ". " << book.getBookTitle() << " (ISBN: " << book.getIsbn() << ")\n";
      }
      cout << "0. Return to search menu\n\n";
      cout << "Select a book: ";

      string selectionInput;
      getline(cin, selectionInput);
      selectionInput = trim(selectionInput);

      int selection = -1;
      if (!selectionInput.empty())
      {
        if (!parseNonNegativeInt(selectionInput, selection))
        {
          selection = -1;
        }
      }

      if (selection == 0)
      {
        returnToSearchMenu = true;
        continue;
      }

      if (selection < 1 || static_cast<size_t>(selection) > matches.size())
      {
        cout << "Please choose a listed option.\n";
        pressEnterToContinue();
        continue;
      }

      clearScreen();
      bookInfo(inventory[matches[selection - 1]]);
      pressEnterToContinue();
    }
  }
}

void addBook()
{
  if (inventory.size() >= kMaxInventory)
  {
    cout << "Inventory full (" << kMaxInventory << "/" << kMaxInventory << "). Cannot add more books.\n";
    return;
  }

  string bookTitle;
  string isbn;
  string author;
  string publisher;
  string dateAdded;
  int qtyOnHand = 0;
  double wholesale = 0.0;
  double retail = 0.0;

  bool hasTitle = false;
  bool hasIsbn = false;
  bool hasAuthor = false;
  bool hasPublisher = false;
  bool hasDate = false;
  bool hasQty = false;
  bool hasWholesale = false;
  bool hasRetail = false;

  auto promptStringField = [](const string &label, string &target, bool &hasValue)
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

      target = trimmed;
      hasValue = true;
      break;
    }
  };

  auto promptDateField = [](string &target, bool &hasValue)
  {
    while (true)
    {
      cout << "Enter Date Added (mm/dd/yyyy): ";
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
        cout << "Please enter the date in mm/dd/yyyy format.\n";
        continue;
      }

      target = trimmed;
      hasValue = true;
      break;
    }
  };

  auto promptIntField = [](const string &label, int &target, bool &hasValue)
  {
    while (true)
    {
      cout << label << ": ";
      string input;
      getline(cin, input);
      string trimmed = trim(input);

      if (trimmed.empty())
      {
        cout << "Please enter a non-negative integer for " << label << ".\n";
        continue;
      }

      int parsed = 0;
      if (!parseNonNegativeInt(trimmed, parsed))
      {
        cout << "Please enter a non-negative integer for " << label << ".\n";
        continue;
      }

      target = parsed;
      hasValue = true;
      break;
    }
  };

  auto promptDoubleField = [](const string &label, double &target, bool &hasValue)
  {
    while (true)
    {
      cout << label << ": ";
      string input;
      getline(cin, input);
      string trimmed = trim(input);

      if (trimmed.empty())
      {
        cout << "Please enter a non-negative number for " << label << ".\n";
        continue;
      }

      double parsed = 0.0;
      if (!parseNonNegativeDouble(trimmed, parsed))
      {
        cout << "Please enter a non-negative number for " << label << ".\n";
        continue;
      }

      target = parsed;
      hasValue = true;
      break;
    }
  };

  bool done = false;

  while (!done)
  {
    clearScreen();
    cout << "Serendipity Booksellers\n\n";
    cout << "Add Book to Inventory\n\n";
    cout << "DATABASE SIZE: " << kMaxInventory
         << "    CURRENT BOOK COUNT: " << BookType::getBookCount() << "\n\n";

    cout << "1. Enter Book Title        : " << formatStringField(bookTitle, hasTitle) << '\n';
    cout << "2. Enter ISBN              : " << formatStringField(isbn, hasIsbn) << '\n';
    cout << "3. Enter Author            : " << formatStringField(author, hasAuthor) << '\n';
    cout << "4. Enter Publisher         : " << formatStringField(publisher, hasPublisher) << '\n';
    cout << "5. Enter Date Added        : " << formatStringField(dateAdded, hasDate) << '\n';
    cout << "6. Enter Quantity on Hand  : " << formatIntField(qtyOnHand, hasQty) << '\n';
    cout << "7. Enter Wholesale Cost    : " << formatMoneyField(wholesale, hasWholesale) << '\n';
    cout << "8. Enter Retail Price      : " << formatMoneyField(retail, hasRetail) << '\n';
    cout << "9. Save Book to Database\n";
    cout << "10. Return to Inventory Menu\n\n";
    cout << "Select an option (1-10): ";

    string choiceInput;
    getline(cin, choiceInput);
    string trimmedChoice = trim(choiceInput);

    int option = -1;
    if (!trimmedChoice.empty())
    {
      if (!parseNonNegativeInt(trimmedChoice, option))
      {
        option = -1;
      }
    }

    if (option < 1 || option > 10)
    {
      cout << "\nPlease enter a number in the range 1 – 10.\n";
      pressEnterToContinue();
      continue;
    }

    switch (option)
    {
      case 1:
        promptStringField("Enter Book Title", bookTitle, hasTitle);
        break;
      case 2:
        promptStringField("Enter ISBN", isbn, hasIsbn);
        break;
      case 3:
        promptStringField("Enter Author", author, hasAuthor);
        break;
      case 4:
        promptStringField("Enter Publisher", publisher, hasPublisher);
        break;
      case 5:
        promptDateField(dateAdded, hasDate);
        break;
      case 6:
        promptIntField("Enter Quantity on Hand", qtyOnHand, hasQty);
        break;
      case 7:
        promptDoubleField("Enter Wholesale Cost", wholesale, hasWholesale);
        break;
      case 8:
        promptDoubleField("Enter Retail Price", retail, hasRetail);
        break;
      case 9:
      {
        vector<string> missingFields;
        if (!hasTitle)      missingFields.push_back("Book Title");
        if (!hasIsbn)       missingFields.push_back("ISBN");
        if (!hasAuthor)     missingFields.push_back("Author");
        if (!hasPublisher)  missingFields.push_back("Publisher");
        if (!hasDate)       missingFields.push_back("Date Added");
        if (!hasQty)        missingFields.push_back("Quantity on Hand");
        if (!hasWholesale)  missingFields.push_back("Wholesale Cost");
        if (!hasRetail)     missingFields.push_back("Retail Price");

        if (!missingFields.empty())
        {
          cout << "\nPlease complete the following fields before saving:\n";
          for (const string &field : missingFields)
          {
            cout << "  - " << field << '\n';
          }
          pressEnterToContinue();
          break;
        }

        if (inventory.size() >= kMaxInventory)
        {
          cout << "\nInventory full (" << kMaxInventory << "/" << kMaxInventory << "). Cannot add more books.\n";
          pressEnterToContinue();
          return;
        }

        if (retail < wholesale)
        {
          cout << "\nWarning: Retail price is less than wholesale cost.\n";
        }

        if (!confirmSave())
        {
          cout << "Record not saved.\n";
          pressEnterToContinue();
          break;
        }

        inventory.emplace_back(bookTitle, isbn, author, publisher, dateAdded, qtyOnHand, wholesale, retail);
        cout << "\nBook added successfully.\n";
        cout << "Inventory count: " << inventory.size() << '/' << kMaxInventory << ".\n";
        if (inventory.size() == kMaxInventory)
        {
          cout << "Inventory now full (" << kMaxInventory << "/" << kMaxInventory << ").\n";
        }
        return;
      }
      case 10:
        cout << "Returning to Inventory Menu without saving.\n";
        return;
    }
  }
}

void editBook()    { cout << "You selected Edit Book.\n"; }
void deleteBook()  { cout << "You selected Delete Book.\n"; }
