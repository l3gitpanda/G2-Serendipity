#include "invmenu.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "bookType.h"
#include "bookinfo.h"
#include "utils.h"

namespace
{
    constexpr std::size_t kMaxInventory = 20;
    // Container choice: std::vector of bookType objects capped at 20 entries.
    std::vector<bookType> inventory;

    enum class FieldChoice
    {
        Title = 1,
        ISBN,
        Author,
        Publisher,
        DateAdded,
        Quantity,
        Wholesale,
        Retail
    };

    struct BookDraft
    {
        std::string title;
        std::string isbn;
        std::string author;
        std::string publisher;
        std::string dateAdded;
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

        [[nodiscard]] bool isComplete() const
        {
            return titleSet && isbnSet && authorSet && publisherSet &&
                   dateSet && quantitySet && wholesaleSet && retailSet;
        }
    };

    void ensureInventoryCapacity()
    {
        if (inventory.capacity() < kMaxInventory)
        {
            inventory.reserve(kMaxInventory);
        }
    }

    std::string fieldLabel(FieldChoice choice)
    {
        switch (choice)
        {
            case FieldChoice::Title:     return "Enter Book Title";
            case FieldChoice::ISBN:      return "Enter ISBN";
            case FieldChoice::Author:    return "Enter Author";
            case FieldChoice::Publisher: return "Enter Publisher";
            case FieldChoice::DateAdded: return "Enter Date Added (mm/dd/yyyy)";
            case FieldChoice::Quantity:  return "Enter Quantity on Hand";
            case FieldChoice::Wholesale: return "Enter Wholesale Cost";
            case FieldChoice::Retail:    return "Enter Retail Price";
        }
        return "";
    }

    void renderAddBookForm(const BookDraft &draft)
    {
        clearScreen();
        std::cout << "Serendipity Booksellers\n\n"
                  << "Inventory Database - Add Book\n\n"
                  << "DATABASE SIZE: " << kMaxInventory << '\n'
                  << "Books in Database: " << bookType::getBookCount() << "\n\n";

        auto fieldLine = [](int index, const std::string &label, const std::string &value)
        {
            std::cout << std::setw(2) << index << ". " << label;
            if (!value.empty())
            {
                std::cout << ": " << value;
            }
            std::cout << '\n';
        };

        fieldLine(1, fieldLabel(FieldChoice::Title), draft.titleSet ? draft.title : "EMPTY");
        fieldLine(2, fieldLabel(FieldChoice::ISBN), draft.isbnSet ? draft.isbn : "EMPTY");
        fieldLine(3, fieldLabel(FieldChoice::Author), draft.authorSet ? draft.author : "EMPTY");
        fieldLine(4, fieldLabel(FieldChoice::Publisher), draft.publisherSet ? draft.publisher : "EMPTY");
        fieldLine(5, fieldLabel(FieldChoice::DateAdded), draft.dateSet ? draft.dateAdded : "EMPTY");
        fieldLine(6, fieldLabel(FieldChoice::Quantity), draft.quantitySet ? std::to_string(draft.quantity) : "0");
        fieldLine(7, fieldLabel(FieldChoice::Wholesale), draft.wholesaleSet ? formatMoney(draft.wholesale) : "$0.00");
        fieldLine(8, fieldLabel(FieldChoice::Retail), draft.retailSet ? formatMoney(draft.retail) : "$0.00");

        std::cout << '\n'
                  << " 9. Save Book to Database\n"
                  << " 0. Return to Inventory Menu\n";
    }

    bool requestLine(const std::string &prompt, std::string &value)
    {
        for (;;)
        {
            std::cout << prompt << ": ";
            if (!std::getline(std::cin, value))
            {
                return false;
            }

            std::string trimmed = trim(value);
            if (trimmed.empty())
            {
                std::cout << prompt << " cannot be blank.\n";
                continue;
            }

            value = trimmed;
            return true;
        }
    }

    bool requestDate(const std::string &prompt, std::string &value)
    {
        for (;;)
        {
            if (!requestLine(prompt, value))
            {
                return false;
            }

            if (!isValidDate(value))
            {
                std::cout << "Please enter the date in mm/dd/yyyy format.\n";
                continue;
            }

            return true;
        }
    }

    bool requestQuantity(const std::string &prompt, int &value)
    {
        std::string input;
        for (;;)
        {
            std::cout << prompt << ": ";
            if (!std::getline(std::cin, input))
            {
                return false;
            }

            int parsed = 0;
            if (!parseNonNegativeInt(input, parsed))
            {
                std::cout << "Please enter a non-negative whole number.\n";
                continue;
            }

            value = parsed;
            return true;
        }
    }

    bool requestMoney(const std::string &prompt, double &value)
    {
        std::string input;
        for (;;)
        {
            std::cout << prompt << ": ";
            if (!std::getline(std::cin, input))
            {
                return false;
            }

            double parsed = 0.0;
            if (!parseNonNegativeDouble(input, parsed))
            {
                std::cout << "Please enter a non-negative number.\n";
                continue;
            }

            value = parsed;
            return true;
        }
    }

    bool promptForField(FieldChoice choice, BookDraft &draft)
    {
        switch (choice)
        {
            case FieldChoice::Title:
                if (!requestLine(fieldLabel(choice), draft.title))
                {
                    return false;
                }
                draft.titleSet = true;
                return true;
            case FieldChoice::ISBN:
                if (!requestLine(fieldLabel(choice), draft.isbn))
                {
                    return false;
                }
                draft.isbnSet = true;
                return true;
            case FieldChoice::Author:
                if (!requestLine(fieldLabel(choice), draft.author))
                {
                    return false;
                }
                draft.authorSet = true;
                return true;
            case FieldChoice::Publisher:
                if (!requestLine(fieldLabel(choice), draft.publisher))
                {
                    return false;
                }
                draft.publisherSet = true;
                return true;
            case FieldChoice::DateAdded:
                if (!requestDate(fieldLabel(choice), draft.dateAdded))
                {
                    return false;
                }
                draft.dateSet = true;
                return true;
            case FieldChoice::Quantity:
                if (!requestQuantity(fieldLabel(choice), draft.quantity))
                {
                    return false;
                }
                draft.quantitySet = true;
                return true;
            case FieldChoice::Wholesale:
                if (!requestMoney(fieldLabel(choice), draft.wholesale))
                {
                    return false;
                }
                draft.wholesaleSet = true;
                return true;
            case FieldChoice::Retail:
                if (!requestMoney(fieldLabel(choice), draft.retail))
                {
                    return false;
                }
                draft.retailSet = true;
                return true;
        }
        return false;
    }

    std::vector<int> searchByTitle(const std::string &query)
    {
        std::vector<int> matches;
        std::string needle = toLowerCopy(trim(query));
        if (needle.empty())
        {
            return matches;
        }

        for (std::size_t index = 0; index < inventory.size(); ++index)
        {
            if (toLowerCopy(inventory[index].getTitle()).find(needle) != std::string::npos)
            {
                matches.push_back(static_cast<int>(index));
            }
        }

        return matches;
    }

    std::vector<int> searchByISBN(const std::string &query)
    {
        std::vector<int> matches;
        std::string needle = trim(query);
        if (needle.empty())
        {
            return matches;
        }

        for (std::size_t index = 0; index < inventory.size(); ++index)
        {
            if (inventory[index].getISBN().find(needle) != std::string::npos)
            {
                matches.push_back(static_cast<int>(index));
            }
        }

        return matches;
    }

    void showSearchResults(const std::vector<int> &matches, const std::string &query)
    {
        bool viewingResults = true;
        while (viewingResults)
        {
            clearScreen();
            std::cout << "Serendipity Booksellers\n\n"
                      << "Inventory Database - Search Results\n\n"
                      << "DATABASE SIZE: " << kMaxInventory << '\n'
                      << "Books in Database: " << bookType::getBookCount() << "\n\n";

            std::cout << "Results for \"" << query << "\"\n\n";

            for (std::size_t i = 0; i < matches.size(); ++i)
            {
                const bookType &book = inventory[matches[i]];
                std::cout << std::setw(2) << (i + 1) << ". " << book.getTitle()
                          << " (ISBN: " << book.getISBN() << ")\n";
            }

            std::cout << " 0. Return to Look Up Menu\n\n"
                      << "Enter your choice: ";

            std::string input;
            if (!std::getline(std::cin, input))
            {
                return;
            }

            int selection = -1;
            if (!parseNonNegativeInt(input, selection))
            {
                std::cout << "Please enter a valid option.\n";
                pressEnterToContinue();
                continue;
            }

            if (selection == 0)
            {
                viewingResults = false;
                continue;
            }

            if (selection < 1 || selection > static_cast<int>(matches.size()))
            {
                std::cout << "Please enter a number between 1 and " << matches.size() << ".\n";
                pressEnterToContinue();
                continue;
            }

            bookInfo(inventory[matches[selection - 1]]);
        }
    }
}

static void printInvMenu()
{
    std::cout << "Serendipity Booksellers\n\n"
              << "Inventory Database\n\n"
              << "1. Look Up a Book\n"
              << "2. Add a Book\n"
              << "3. Edit a Book's Record\n"
              << "4. Delete a Book\n"
              << "5. Return to the Main Menu\n\n"
              << "Enter Your Choice: ";
}

void invMenu()
{
    ensureInventoryCapacity();

    bool running = true;
    while (running)
    {
        clearScreen();
        printInvMenu();

        std::string input;
        if (!std::getline(std::cin, input))
        {
            return;
        }

        if (input.size() != 1 || input[0] < '1' || input[0] > '5')
        {
            std::cout << "\nPlease enter a number in the range 1 – 5.\n";
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
    if (inventory.empty())
    {
        std::cout << "Inventory is empty. Add books before searching.\n";
        return;
    }

    bool exitMenu = false;
    while (!exitMenu)
    {
        clearScreen();
        std::cout << "Serendipity Booksellers\n\n"
                  << "Inventory Database - Look Up Book\n\n"
                  << "DATABASE SIZE: " << kMaxInventory << '\n'
                  << "Books in Database: " << bookType::getBookCount() << "\n\n"
                  << "1. Search by Title\n"
                  << "2. Search by ISBN\n"
                  << "0. Return to Inventory Menu\n\n"
                  << "Enter your choice: ";

        std::string choice;
        if (!std::getline(std::cin, choice))
        {
            return;
        }

        choice = trim(choice);
        if (choice == "0")
        {
            exitMenu = true;
            continue;
        }

        if (choice != "1" && choice != "2")
        {
            std::cout << "Please enter 1, 2, or 0.\n";
            pressEnterToContinue();
            continue;
        }

        std::string prompt = (choice == "1") ? "Enter title to search for" : "Enter ISBN to search for";
        std::cout << prompt << ": ";

        std::string query;
        if (!std::getline(std::cin, query))
        {
            return;
        }

        query = trim(query);
        if (query.empty())
        {
            std::cout << "Search text cannot be blank.\n";
            pressEnterToContinue();
            continue;
        }

        std::vector<int> matches = (choice == "1") ? searchByTitle(query) : searchByISBN(query);
        if (matches.empty())
        {
            std::cout << "\nNot in inventory.\n";
            pressEnterToContinue();
            continue;
        }

        showSearchResults(matches, query);
    }
}

void addBook()
{
    ensureInventoryCapacity();

    if (inventory.size() >= kMaxInventory)
    {
        std::cout << "Inventory full (20/20). Cannot add more books.\n";
        return;
    }

    BookDraft draft;

    clearScreen();
    std::cout << "Serendipity Booksellers\n\n"
              << "Inventory Database - Add Book\n\n"
              << "DATABASE SIZE: " << kMaxInventory << '\n'
              << "Books in Database: " << bookType::getBookCount() << "\n\n";

    for (int option = static_cast<int>(FieldChoice::Title);
         option <= static_cast<int>(FieldChoice::Retail); ++option)
    {
        if (!promptForField(static_cast<FieldChoice>(option), draft))
        {
            return;
        }
    }

    bool editing = true;
    while (editing)
    {
        renderAddBookForm(draft);
        std::cout << "\nEnter your choice: ";

        std::string choice;
        if (!std::getline(std::cin, choice))
        {
            return;
        }

        choice = trim(choice);
        if (choice.empty())
        {
            std::cout << "Please select a menu option.\n";
            pressEnterToContinue();
            continue;
        }

        if (choice == "0")
        {
            std::cout << "\nAdd Book cancelled. Returning to Inventory Menu.\n";
            return;
        }

        if (choice == "9")
        {
            if (!draft.isComplete())
            {
                std::cout << "All fields must be completed before saving.\n";
                pressEnterToContinue();
                continue;
            }

            if (inventory.size() >= kMaxInventory)
            {
                std::cout << "Inventory full (20/20). Cannot add more books.\n";
                return;
            }

            inventory.emplace_back(
                draft.isbn,
                draft.title,
                draft.author,
                draft.publisher,
                draft.dateAdded,
                draft.quantity,
                draft.wholesale,
                draft.retail);

            std::cout << "\nBook saved to database.\n"
                      << "Books in Database: " << bookType::getBookCount()
                      << '/' << kMaxInventory << "\n";

            if (draft.retail < draft.wholesale)
            {
                std::cout << "Warning: Retail price is less than wholesale cost.\n";
            }

            return;
        }

        int menuSelection = -1;
        if (!parseNonNegativeInt(choice, menuSelection) ||
            menuSelection < 1 || menuSelection > 8)
        {
            std::cout << "Please choose an option from the menu.\n";
            pressEnterToContinue();
            continue;
        }

        if (!promptForField(static_cast<FieldChoice>(menuSelection), draft))
        {
            return;
        }
    }
}

void editBook()
{
    std::cout << "You selected Edit Book.\n";
}

void deleteBook()
{
    std::cout << "You selected Delete Book.\n";
}
