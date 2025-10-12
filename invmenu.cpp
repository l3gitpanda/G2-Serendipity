/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 10/10/2025
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

std::vector<BookType> searchForBookByTitle(const string &searchSubstring) {
  vector<BookType> results;
  for (const auto &book : inventory) {
    if (book.getBookTitle().find(searchSubstring) != string::npos) {
      results.push_back(book);
    }
  }
  return results;
}

std::vector<BookType> searchForBookByISBN(const string &searchISBN) {
  vector<BookType> results;
  for (const auto &book : inventory) {
    if (book.getIsbn().find(searchISBN) != string::npos) { // Partial match
      results.push_back(book);
    }
  }
  return results;
}

void chooseBookFromResults(vector<BookType> &searchResults) {
  bool loopShouldEnd = false;
  do {
    clearScreen(); // Clear the screen before printing options
        const vector<string> bookNavigationOptions;

    for (const auto &book : searchResults) {
      bookNavigationOptions.push_back(book.getBookTitle());
    } // Get titles of books for menu

    bookNavigationOptions.push_back("Return to Search Menu");

    navigationMenu resultsMenu(
      "Search Results",
      bookNavigationOptions
    );

    resultsMenu.print(12, 5);

    if (searchResults.empty()) {
      cout << "Not in inventory.\n";
      pressEnterToContinue();
      loopShouldEnd = true;
      continue; // Go back to search menu
    }

    char bookNavigationChoice;

    cin >> bookNavigationChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (bookNavigationChoice < '1' || bookNavigationChoice > static_cast<char>('0' + searchResults.size() + 1)) {
      cout << "Please enter a valid option.\n";
      pressEnterToContinue();
      continue; // Reprint menu
    }
    
    if (bookNavigationChoice == static_cast<char>('0' + searchResults.size() + 1)) {
      // Return to search menu
      loopShouldEnd = true;
      continue;
    }

    /*
    bookInfo(searchResults[
      bookNavigationChoice - 
      static_cast<int>('1')
    ]);
    */ 
  } while (!loopShouldEnd);
}

// Inventory stubs (navigation-only)
void lookUpBook()  { 
  bool loopShouldEnd = false;
  do {
    clearScreen(); // Clear the screen before printing options

    vector<BookType> searchResults;
  
    cout << "Look Up a Book.\n"; 
    cout << "---------------\n";

    navigationMenu searchMenu(
      "Search Menu",
      {
        "Search by Title",
        "Search by ISBN",
        "Return to Inventory Menu"
      }
    );

    searchMenu.print(12, 5);

    char navigationInput;
    string searchString;

    cin >> navigationInput;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (navigationInput) {
      case '1':
        cout << "Enter title to search for: ";
        getline(cin, searchString);
        searchResults = searchForBookByTitle(searchString);
        break;
      case '2':
        cout << "Enter ISBN to search for: ";
        getline(cin, searchString);
        searchResults = searchForBookByISBN(searchString);
        break;
      case '3':
        cout << "Returning to Inventory Menu.\n";
        loopShouldEnd = true;
        pressEnterToContinue();
        continue; // Skip the rest of the loop
        break;
    }

    chooseBookFromResults(searchResults);


  } while (!loopShouldEnd);
  

}


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