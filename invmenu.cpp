/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 10/10/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/

#include "invmenu.h"

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
#include "utils.h"

using namespace std;

namespace
{
  // Container choice: std::vector<BookType>
  constexpr size_t kMaxInventory = 20;
  vector<BookType> inventory;

  enum class Field
  {
    kTitle = 0,
    kIsbn,
    kAuthor,
    kPublisher,
    kDateAdded,
    kQuantity,
    kWholesale,
    kRetail
  };

  struct AddBookFields
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
    bool quantitySet = false;
    bool wholesaleSet = false;
    bool retailSet = false;
  };

  void ensureReserved()
  {
    if (inventory.capacity() < kMaxInventory)
    {
      inventory.reserve(kMaxInventory);
    }
  }

  void printInventoryHeader()
  {
    cout << left << setw(18) << "DATABASE SIZE:" << kMaxInventory << '\n';
    cout << left << setw(18) << "CURRENT COUNT:" << BookType::getBookCount() << "\n\n";
  }

  string toLower(string value)
  {
    transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
      return static_cast<char>(tolower(ch));
    });
    return value;
  }

  bool isAllDigits(const string &text)
  {
    return !text.empty() && all_of(text.begin(), text.end(), [](unsigned char ch) {
      return isdigit(ch) != 0;
    });
  }

  bool isValidDate(const string &value)
  {
    if (value.size() != 10 || value[2] != '/' || value[5] != '/')
    {
      return false;
    }

    const string monthText = value.substr(0, 2);
    const string dayText = value.substr(3, 2);
    const string yearText = value.substr(6, 4);

    if (!isAllDigits(monthText) || !isAllDigits(dayText) || !isAllDigits(yearText))
    {
      return false;
    }

    const int month = stoi(monthText);
    const int day = stoi(dayText);
    const int year = stoi(yearText);

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

  bool promptString(const string &label, string &out)
  {
    while (true)
    {
      cout << label << ": ";
      string input;
      getline(cin, input);

      const string trimmed = trim(input);
      if (trimmed.empty())
      {
        cout << label << " cannot be blank.\n";
        continue;
      }

      out = trimmed;
      return true;
    }
  }

  bool promptDate(string &out)
  {
    while (true)
    {
      cout << "Enter Date Added (mm/dd/yyyy): ";
      string input;
      getline(cin, input);

      const string trimmed = trim(input);
      if (trimmed.empty())
      {
        cout << "Date Added cannot be blank.\n";
        continue;
      }

      if (!isValidDate(trimmed))
      {
        cout << "Please enter a date in mm/dd/yyyy format.\n";
        continue;
      }

      out = trimmed;
      return true;
    }
  }

  bool promptQuantity(int &quantity)
  {
    while (true)
    {
      cout << "Enter Quantity on Hand: ";
      string input;
      getline(cin, input);

      const string trimmed = trim(input);
      if (trimmed.empty())
      {
        cout << "Quantity on Hand cannot be blank.\n";
        continue;
      }

      int parsed = 0;
      if (!parseNonNegativeInt(trimmed, parsed))
      {
        cout << "Please enter a non-negative integer.\n";
        continue;
      }

      quantity = parsed;
      return true;
    }
  }

  bool promptCurrency(const string &label, double &value)
  {
    while (true)
    {
      cout << "Enter " << label << ": ";
      string input;
      getline(cin, input);

      const string trimmed = trim(input);
      if (trimmed.empty())
      {
        cout << label << " cannot be blank.\n";
        continue;
      }

      double parsed = 0.0;
      if (!parseNonNegativeDouble(trimmed, parsed))
      {
        cout << "Please enter a non-negative number.\n";
        continue;
      }

      value = parsed;
      return true;
    }
  }

  bool promptField(AddBookFields &fields, Field field)
  {
    switch (field)
    {
      case Field::kTitle:
        if (!promptString("Enter Book Title", fields.title))
        {
          return false;
        }
        fields.titleSet = true;
        return true;
      case Field::kIsbn:
        if (!promptString("Enter ISBN", fields.isbn))
        {
          return false;
        }
        fields.isbnSet = true;
        return true;
      case Field::kAuthor:
        if (!promptString("Enter Author", fields.author))
        {
          return false;
        }
        fields.authorSet = true;
        return true;
      case Field::kPublisher:
        if (!promptString("Enter Publisher", fields.publisher))
        {
          return false;
        }
        fields.publisherSet = true;
        return true;
      case Field::kDateAdded:
        if (!promptDate(fields.dateAdded))
        {
          return false;
        }
        fields.dateSet = true;
        return true;
      case Field::kQuantity:
        if (!promptQuantity(fields.quantity))
        {
          return false;
        }
        fields.quantitySet = true;
        return true;
      case Field::kWholesale:
        if (!promptCurrency("Wholesale Cost", fields.wholesale))
        {
          return false;
        }
        fields.wholesaleSet = true;
        return true;
      case Field::kRetail:
        if (!promptCurrency("Retail Price", fields.retail))
        {
          return false;
        }
        fields.retailSet = true;
        return true;
    }

    return false;
  }

  bool allFieldsSet(const AddBookFields &fields)
  {
    return fields.titleSet && fields.isbnSet && fields.authorSet && fields.publisherSet &&
           fields.dateSet && fields.quantitySet && fields.wholesaleSet && fields.retailSet;
  }

  string displayStringField(const string &value, bool set)
  {
    return set ? value : "EMPTY";
  }

  string displayQuantity(int value, bool set)
  {
    return set ? to_string(value) : "0";
  }

  string displayCurrency(double value, bool set)
  {
    ostringstream oss;
    oss << '$' << fixed << setprecision(2) << (set ? value : 0.0);
    return oss.str();
  }

  void printAddBookMenu(const AddBookFields &fields)
  {
    printInventoryHeader();
    cout << "Add Book" << '\n';
    cout << "--------" << "\n\n";

    cout << "1. Enter Book Title         : " << displayStringField(fields.title, fields.titleSet) << '\n';
    cout << "2. Enter ISBN               : " << displayStringField(fields.isbn, fields.isbnSet) << '\n';
    cout << "3. Enter Author             : " << displayStringField(fields.author, fields.authorSet) << '\n';
    cout << "4. Enter Publisher          : " << displayStringField(fields.publisher, fields.publisherSet) << '\n';
    cout << "5. Enter Date Added         : " << displayStringField(fields.dateAdded, fields.dateSet) << '\n';
    cout << "6. Enter Quantity on Hand   : " << displayQuantity(fields.quantity, fields.quantitySet) << '\n';
    cout << "7. Enter Wholesale Cost     : " << displayCurrency(fields.wholesale, fields.wholesaleSet) << '\n';
    cout << "8. Enter Retail Price       : " << displayCurrency(fields.retail, fields.retailSet) << '\n';
    cout << "9. Save Book to Database" << '\n';
    cout << "10. Return to Inventory Menu" << '\n';
  }

  void collectInitialFields(AddBookFields &fields)
  {
    clearScreen();
    printInventoryHeader();
    cout << "Add Book" << '\n';
    cout << "--------" << "\n\n";
    cout << "Paste the record values using the prompts below." << "\n\n";

    promptField(fields, Field::kTitle);
    promptField(fields, Field::kIsbn);
    promptField(fields, Field::kAuthor);
    promptField(fields, Field::kPublisher);
    promptField(fields, Field::kDateAdded);
    promptField(fields, Field::kQuantity);
    promptField(fields, Field::kWholesale);
    promptField(fields, Field::kRetail);
  }

  vector<size_t> searchByTitle(const string &query)
  {
    vector<size_t> matches;
    const string loweredQuery = toLower(query);

    for (size_t index = 0; index < inventory.size(); ++index)
    {
      const string loweredTitle = toLower(inventory[index].getBookTitle());
      if (loweredTitle.find(loweredQuery) != string::npos)
      {
        matches.push_back(index);
      }
    }

    return matches;
  }

  vector<size_t> searchByIsbn(const string &query)
  {
    vector<size_t> matches;
    const string trimmed = trim(query);

    for (size_t index = 0; index < inventory.size(); ++index)
    {
      if (inventory[index].getIsbn().find(trimmed) != string::npos)
      {
        matches.push_back(index);
      }
    }

    return matches;
  }

  void showSearchResults(const vector<size_t> &matches, const string &searchTerm)
  {
    if (matches.empty())
    {
      cout << "\n'" << searchTerm << "' is not in the inventory.\n";
      return;
    }

    while (true)
    {
      clearScreen();
      printInventoryHeader();
      cout << "Search Results" << '\n';
      cout << "--------------" << '\n';
      cout << "Matches for '" << searchTerm << "':" << "\n\n";

      for (size_t i = 0; i < matches.size(); ++i)
      {
        const BookType &book = inventory[matches[i]];
        cout << setw(2) << (i + 1) << ". " << book.getBookTitle() << " (ISBN: " << book.getIsbn() << ")\n";
      }

      cout << '\n' << (matches.size() + 1) << ". Return to Search Menu\n";
      cout << "Select an option: ";

      string choiceText;
      getline(cin, choiceText);

      int choice = 0;
      stringstream parser(choiceText);
      if (!(parser >> choice) || choice < 1 || choice > static_cast<int>(matches.size()) + 1)
      {
        cout << "Please enter a valid option.\n";
        pressEnterToContinue();
        continue;
      }

      if (choice == static_cast<int>(matches.size()) + 1)
      {
        return;
      }

      clearScreen();
      bookInfo(inventory[matches[static_cast<size_t>(choice - 1)]]);
      cout << '\n';
      pressEnterToContinue();
    }
  }

  bool validateBeforeSave(const AddBookFields &fields)
  {
    if (!allFieldsSet(fields))
    {
      cout << "\nAll fields must be completed before saving.\n";
      return false;
    }

    return true;
  }

  void saveBook(const AddBookFields &fields)
  {
    BookType record(fields.title, fields.isbn, fields.author, fields.publisher, fields.dateAdded, fields.quantity,
                    fields.wholesale, fields.retail);
    inventory.push_back(record);

    if (fields.retail < fields.wholesale)
    {
      cout << "\nWarning: retail price is below wholesale cost.\n";
    }
  }

  void printInventoryMenu()
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
}

void invMenu()
{
  ensureReserved();

  bool running = true;
  while (running)
  {
    clearScreen();
    printInventoryMenu();

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
        running = false;
        break;
    }
  }
}

void lookUpBook()
{
  while (true)
  {
    clearScreen();
    printInventoryHeader();
    cout << "Look Up Book" << '\n';
    cout << "------------" << '\n';
    cout << "1. Search by Title\n";
    cout << "2. Search by ISBN\n";
    cout << "3. Return to Inventory Menu\n\n";
    cout << "Enter Your Choice: ";

    string choiceText;
    getline(cin, choiceText);

    if (choiceText.size() != 1 || choiceText[0] < '1' || choiceText[0] > '3')
    {
      cout << "\nPlease enter 1, 2, or 3.\n";
      pressEnterToContinue();
      continue;
    }

    if (choiceText[0] == '3')
    {
      return;
    }

    cout << ((choiceText[0] == '1') ? "Enter title to search: " : "Enter ISBN to search: ");
    string query;
    getline(cin, query);

    const string trimmedQuery = trim(query);
    if (trimmedQuery.empty())
    {
      cout << "\nSearch text cannot be empty.\n";
      pressEnterToContinue();
      continue;
    }

    vector<size_t> matches = (choiceText[0] == '1') ? searchByTitle(trimmedQuery) : searchByIsbn(trimmedQuery);
    showSearchResults(matches, trimmedQuery);
    pressEnterToContinue();
  }
}

void addBook()
{
  if (inventory.size() >= kMaxInventory)
  {
    cout << "Inventory full (20/20). Cannot add more books.\n";
    return;
  }

  AddBookFields fields;
  collectInitialFields(fields);

  while (true)
  {
    clearScreen();
    printAddBookMenu(fields);
    cout << "\nSelect an option: ";

    string choiceText;
    getline(cin, choiceText);

    int option = 0;
    stringstream parser(choiceText);
    if (!(parser >> option) || option < 1 || option > 10)
    {
      cout << "\nPlease enter a number between 1 and 10.\n";
      pressEnterToContinue();
      continue;
    }

    if (option == 10)
    {
      cout << "\nReturning to Inventory Menu without saving.\n";
      return;
    }

    if (option >= 1 && option <= 8)
    {
      promptField(fields, static_cast<Field>(option - 1));
      continue;
    }

    if (option == 9)
    {
      if (!validateBeforeSave(fields))
      {
        pressEnterToContinue();
        continue;
      }

      saveBook(fields);
      cout << "\nBook added successfully.\n";
      cout << "Inventory count: " << inventory.size() << '/' << kMaxInventory << "\n";
      if (inventory.size() == kMaxInventory)
      {
        cout << "Inventory now full (20/20).\n";
      }
      return;
    }
  }
}

void editBook()
{
  cout << "Edit Book is not implemented in this lab.\n";
}

void deleteBook()
{
  cout << "Delete Book is not implemented in this lab.\n";
}
