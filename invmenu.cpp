/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 10/13/2025
  Purpose: Inventory Menu
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp -o serendipity.out
*/

#include "invmenu.h"

#include <deque>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "bookType.h"
#include "bookinfo.h"
#include "utils.h"

namespace
{
    constexpr std::size_t kMaxInventory = 20;
    // Inventory container: std::vector limited to 20 entries.
    std::vector<bookType> inventory;
    std::deque<std::string> bufferedInputs;

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

    enum class PromptResult
    {
        Success,
        Cancel
    };

    enum class SearchType
    {
        Title = 1,
        ISBN
    };

    enum class DuplicateResolution
    {
        CancelAdd,
        EditFields,
        CompletedAdd
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

    std::string fieldName(FieldChoice choice)
    {
        switch (choice)
        {
            case FieldChoice::Title:     return "Book Title";
            case FieldChoice::ISBN:      return "ISBN";
            case FieldChoice::Author:    return "Author";
            case FieldChoice::Publisher: return "Publisher";
            case FieldChoice::DateAdded: return "Date Added";
            case FieldChoice::Quantity:  return "Quantity on Hand";
            case FieldChoice::Wholesale: return "Wholesale Cost";
            case FieldChoice::Retail:    return "Retail Price";
        }
        return "";
    }

    void captureBufferedInputFromStream()
    {
        while (std::cin.rdbuf()->in_avail() > 0)
        {
            std::string nextLine;
            if (!std::getline(std::cin, nextLine))
            {
                break;
            }

            bufferedInputs.push_back(nextLine);
        }
    }

    std::optional<std::string> acquireNextInput(const std::string &prompt)
    {
        if (!bufferedInputs.empty())
        {
            std::string value = bufferedInputs.front();
            bufferedInputs.pop_front();

            std::cout << prompt << " -> " << value << '\n';
            return value;
        }

        std::cout << prompt << " -> ";

        std::string input;
        if (!std::getline(std::cin, input))
        {
            return std::nullopt;
        }

        captureBufferedInputFromStream();
        return input;
    }

    void renderAddBookForm(const BookDraft &draft, const std::string &statusMessage)
    {
        clearScreen();

        std::cout << "SERENDIPITY BOOKSELLERS\n\n"
                  << "ADD BOOK\n\n"
                  << "DATABASE SIZE: " << kMaxInventory
                  << " CURRENT BOOK COUNT: " << bookType::getBookCount() << "\n\n";

        if (!statusMessage.empty())
        {
            std::cout << statusMessage << "\n\n";
        }

        std::cout << "--PENDING VALUES--\n";

        auto fieldLine = [](int index, const std::string &label, const std::string &value)
        {
            std::ios::fmtflags flags = std::cout.flags();
            std::cout << ' ' << index << ") " << std::left << std::setw(34) << label
                      << "-> " << value << '\n';
            std::cout.flags(flags);
        };

        fieldLine(1, fieldLabel(FieldChoice::Title), draft.titleSet ? draft.title : "--EMPTY--");
        fieldLine(2, fieldLabel(FieldChoice::ISBN), draft.isbnSet ? draft.isbn : "--EMPTY--");
        fieldLine(3, fieldLabel(FieldChoice::Author), draft.authorSet ? draft.author : "--EMPTY--");
        fieldLine(4, fieldLabel(FieldChoice::Publisher), draft.publisherSet ? draft.publisher : "--EMPTY--");
        fieldLine(5, fieldLabel(FieldChoice::DateAdded), draft.dateSet ? draft.dateAdded : "--EMPTY--");
        fieldLine(6, fieldLabel(FieldChoice::Quantity), draft.quantitySet ? std::to_string(draft.quantity) : "0");
        fieldLine(7, fieldLabel(FieldChoice::Wholesale), draft.wholesaleSet ? formatMoney(draft.wholesale) : "$0.00");
        fieldLine(8, fieldLabel(FieldChoice::Retail), draft.retailSet ? formatMoney(draft.retail) : "$0.00");

        std::cout << '\n'
                  << " 9) Save Book to Database\n"
                  << " 0) Return to Inventory Menu\n";
    }

    PromptResult promptString(const std::string &prompt, std::string &value)
    {
        for (;;)
        {
            std::optional<std::string> input = acquireNextInput(prompt);
            if (!input.has_value())
            {
                return PromptResult::Cancel;
            }

            std::string trimmed = trim(*input);
            if (trimmed.empty())
            {
                return PromptResult::Cancel;
            }

            value = trimmed;
            return PromptResult::Success;
        }
    }

    PromptResult promptDate(const std::string &prompt, std::string &value)
    {
        for (;;)
        {
            std::optional<std::string> input = acquireNextInput(prompt);
            if (!input.has_value())
            {
                return PromptResult::Cancel;
            }

            std::string trimmed = trim(*input);
            if (trimmed.empty())
            {
                return PromptResult::Cancel;
            }

            if (!isValidDate(trimmed))
            {
                std::cout << "Please enter the date in mm/dd/yyyy format.\n";
                continue;
            }

            value = trimmed;
            return PromptResult::Success;
        }
    }

    PromptResult promptQuantity(const std::string &prompt, int &value)
    {
        for (;;)
        {
            std::optional<std::string> input = acquireNextInput(prompt);
            if (!input.has_value())
            {
                return PromptResult::Cancel;
            }

            std::string trimmed = trim(*input);
            if (trimmed.empty())
            {
                return PromptResult::Cancel;
            }

            int parsed = 0;
            if (!parseNonNegativeInt(trimmed, parsed))
            {
                std::cout << "Please enter a non-negative whole number.\n";
                continue;
            }

            value = parsed;
            return PromptResult::Success;
        }
    }

    PromptResult promptMoney(const std::string &prompt, double &value)
    {
        for (;;)
        {
            std::optional<std::string> input = acquireNextInput(prompt);
            if (!input.has_value())
            {
                return PromptResult::Cancel;
            }

            std::string trimmed = trim(*input);
            if (trimmed.empty())
            {
                return PromptResult::Cancel;
            }

            double parsed = 0.0;
            if (!parseNonNegativeDouble(trimmed, parsed))
            {
                std::cout << "Please enter a non-negative number.\n";
                continue;
            }

            value = parsed;
            return PromptResult::Success;
        }
    }

    PromptResult promptForField(FieldChoice choice, BookDraft &draft)
    {
        switch (choice)
        {
            case FieldChoice::Title:
            {
                std::string value;
                PromptResult result = promptString(fieldLabel(choice), value);
                if (result == PromptResult::Success)
                {
                    draft.title    = value;
                    draft.titleSet = true;
                }
                return result;
            }
            case FieldChoice::ISBN:
            {
                std::string value;
                PromptResult result = promptString(fieldLabel(choice), value);
                if (result == PromptResult::Success)
                {
                    draft.isbn    = value;
                    draft.isbnSet = true;
                }
                return result;
            }
            case FieldChoice::Author:
            {
                std::string value;
                PromptResult result = promptString(fieldLabel(choice), value);
                if (result == PromptResult::Success)
                {
                    draft.author    = value;
                    draft.authorSet = true;
                }
                return result;
            }
            case FieldChoice::Publisher:
            {
                std::string value;
                PromptResult result = promptString(fieldLabel(choice), value);
                if (result == PromptResult::Success)
                {
                    draft.publisher    = value;
                    draft.publisherSet = true;
                }
                return result;
            }
            case FieldChoice::DateAdded:
            {
                std::string value;
                PromptResult result = promptDate(fieldLabel(choice), value);
                if (result == PromptResult::Success)
                {
                    draft.dateAdded = value;
                    draft.dateSet   = true;
                }
                return result;
            }
            case FieldChoice::Quantity:
            {
                int value = 0;
                PromptResult result = promptQuantity(fieldLabel(choice), value);
                if (result == PromptResult::Success)
                {
                    draft.quantity    = value;
                    draft.quantitySet = true;
                }
                return result;
            }
            case FieldChoice::Wholesale:
            {
                double value = 0.0;
                PromptResult result = promptMoney(fieldLabel(choice), value);
                if (result == PromptResult::Success)
                {
                    draft.wholesale    = value;
                    draft.wholesaleSet = true;
                }
                return result;
            }
            case FieldChoice::Retail:
            {
                double value = 0.0;
                PromptResult result = promptMoney(fieldLabel(choice), value);
                if (result == PromptResult::Success)
                {
                    draft.retail    = value;
                    draft.retailSet = true;
                }
                return result;
            }
        }

        return PromptResult::Cancel;
    }

    std::vector<int> searchInventory(const std::string &query, SearchType type)
    {
        std::vector<int> matches;
        std::string needle = toLowerCopy(trim(query));
        if (needle.empty())
        {
            return matches;
        }

        for (std::size_t index = 0; index < inventory.size(); ++index)
        {
            const bookType &book = inventory[index];
            std::string haystack =
                (type == SearchType::Title) ? book.getTitle() : book.getISBN();

            if (toLowerCopy(haystack).find(needle) != std::string::npos)
            {
                matches.push_back(static_cast<int>(index));
            }
        }

        return matches;
    }

    int showSearchResults(const std::vector<int> &matches,
                          const std::string        &query,
                          SearchType                type)
    {
        while (true)
        {
            clearScreen();

            std::cout << "Serendipity Booksellers\n\n"
                      << "Inventory Database - Search Results\n\n"
                      << "DATABASE SIZE: " << kMaxInventory << '\n'
                      << "Books in Database: " << bookType::getBookCount() << "\n\n"
                      << "Results for "
                      << ((type == SearchType::Title) ? "Title" : "ISBN")
                      << " \"" << query << "\"\n\n";

            for (std::size_t i = 0; i < matches.size(); ++i)
            {
                const bookType &book = inventory[matches[i]];
                std::cout << std::setw(2) << (i + 1) << ") "
                          << book.getTitle() << " — " << book.getAuthor()
                          << " — " << book.getISBN()
                          << " — Qty: " << book.getQtyOnHand()
                          << " — " << formatMoney(book.getRetail()) << '\n';
            }

            std::cout << "\n0) Cancel and return to Inventory Menu\n"
                      << "Select # to view details: ";

            std::string input;
            if (!std::getline(std::cin, input))
            {
                return -1;
            }

            input = trim(input);
            if (input.empty())
            {
                std::cout << "Please enter a selection.\n";
                pressEnterToContinue();
                continue;
            }

            int selection = 0;
            if (!parseNonNegativeInt(input, selection))
            {
                std::cout << "Please enter a whole number.\n";
                pressEnterToContinue();
                continue;
            }

            if (selection == 0)
            {
                return -1;
            }

            if (selection < 1 || selection > static_cast<int>(matches.size()))
            {
                std::cout << "Please choose a number between 1 and " << matches.size() << ".\n";
                pressEnterToContinue();
                continue;
            }

            int inventoryIndex = matches[selection - 1];
            bookInfo(inventory[inventoryIndex]);
            return inventoryIndex;
        }
    }

    std::optional<std::size_t> findBookIndexByISBN(const std::string &isbn)
    {
        std::string needle = toLowerCopy(trim(isbn));
        if (needle.empty())
        {
            return std::nullopt;
        }

        for (std::size_t index = 0; index < inventory.size(); ++index)
        {
            std::string candidate = toLowerCopy(trim(inventory[index].getISBN()));
            if (candidate == needle)
            {
                return index;
            }
        }

        return std::nullopt;
    }

    DuplicateResolution resolveDuplicateISBN(std::size_t existingIndex, const BookDraft &draft)
    {
        const bookType &existing = inventory[existingIndex];

        while (true)
        {
            clearScreen();

            std::cout << "A book with this ISBN already exists.\n\n"
                      << "Existing Record:\n"
                      << "Title : " << existing.getTitle() << '\n'
                      << "Author: " << existing.getAuthor() << '\n'
                      << "ISBN  : " << existing.getISBN() << '\n'
                      << "Qty   : " << existing.getQtyOnHand() << '\n'
                      << "Retail: " << formatMoney(existing.getRetail()) << "\n\n"
                      << "Options:\n"
                      << " 1) Cancel add and return to Inventory Menu\n"
                      << " 2) Edit fields before saving\n"
                      << " 3) Increase quantity on existing record\n\n"
                      << "Choice: ";

            std::string input;
            if (!std::getline(std::cin, input))
            {
                return DuplicateResolution::CancelAdd;
            }

            input = trim(input);
            if (input == "1")
            {
                return DuplicateResolution::CancelAdd;
            }

            if (input == "2")
            {
                return DuplicateResolution::EditFields;
            }

            if (input == "3")
            {
                inventory[existingIndex].setQtyOnHand(existing.getQtyOnHand() + draft.quantity);

                std::cout << "\nQuantity increased by " << draft.quantity
                          << ". New quantity: " << inventory[existingIndex].getQtyOnHand() << ".\n";
                pressEnterToContinue();
                return DuplicateResolution::CompletedAdd;
            }

            std::cout << "Please enter 1, 2, or 3.\n";
            pressEnterToContinue();
        }
    }
} // namespace

static void printInvMenu()
{
    std::cout << "Serendipity Booksellers\n\n"
              << "Inventory Menu\n\n"
              << "1) Look Up Book\n"
              << "2) Add Book\n"
              << "0) Return to Main Menu\n\n"
              << "Choice: ";
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

        input = trim(input);
        if (input.size() != 1 || (input[0] != '0' && input[0] != '1' && input[0] != '2'))
        {
            std::cout << "\nPlease enter 0, 1, or 2.\n";
            pressEnterToContinue();
            continue;
        }

        switch (input[0])
        {
            case '1':
            {
                clearScreen();
                int selectedIndex = lookUpBook();
                if (selectedIndex == -1)
                {
                    pressEnterToContinue();
                }
                break;
            }
            case '2':
                clearScreen();
                addBook();
                break;
            case '0':
                running = false;
                break;
        }
    }
}

int lookUpBook()
{
    if (inventory.empty())
    {
        std::cout << "Inventory is empty. Add books before searching.\n";
        return -1;
    }

    while (true)
    {
        clearScreen();
        std::cout << "Serendipity Booksellers\n\n"
                  << "Inventory Database - Look Up Book\n\n"
                  << "DATABASE SIZE: " << kMaxInventory << '\n'
                  << "Books in Database: " << bookType::getBookCount() << "\n\n"
                  << "Search by:\n"
                  << " 1) Title\n"
                  << " 2) ISBN\n"
                  << " 0) Return to Inventory Menu\n\n"
                  << "Choice: ";

        std::string modeInput;
        if (!std::getline(std::cin, modeInput))
        {
            return -1;
        }

        modeInput = trim(modeInput);
        if (modeInput.empty() || modeInput == "0")
        {
            std::cout << "Look Up Book cancelled.\n";
            return -1;
        }

        SearchType mode;
        if (modeInput == "1")
        {
            mode = SearchType::Title;
        }
        else if (modeInput == "2")
        {
            mode = SearchType::ISBN;
        }
        else
        {
            std::cout << "\nPlease enter 0, 1, or 2.\n";
            pressEnterToContinue();
            continue;
        }

        std::cout << ((mode == SearchType::Title) ? "Enter title" : "Enter ISBN")
                  << " (press ENTER to cancel): ";

        std::string query;
        if (!std::getline(std::cin, query))
        {
            return -1;
        }

        query = trim(query);
        if (query.empty())
        {
            std::cout << "Look Up Book cancelled.\n";
            return -1;
        }

        std::vector<int> matches = searchInventory(query, mode);
        if (matches.empty())
        {
            std::cout << "\nNo matches found.\n";
            pressEnterToContinue();
            continue;
        }

        int result = showSearchResults(matches, query, mode);
        if (result != -1)
        {
            return result;
        }
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
    std::string statusMessage = "Select a field number to enter information. Leave a field blank while editing to cancel that entry.";

    while (true)
    {
        renderAddBookForm(draft, statusMessage);
        std::cout << "\nChoice (0-9): ";

        std::string choice;
        if (!std::getline(std::cin, choice))
        {
            return;
        }

        choice = trim(choice);
        if (choice.empty())
        {
            statusMessage = "Please choose a menu option.";
            continue;
        }

        if (choice == "0")
        {
            std::cout << "\nReturning to Inventory Menu.\n";
            return;
        }

        if (choice == "9")
        {
            if (!draft.isComplete())
            {
                statusMessage = "All fields must be completed before saving.";
                continue;
            }

            if (inventory.size() >= kMaxInventory)
            {
                statusMessage = "Inventory full (20/20). Cannot add more books.";
                continue;
            }

            if (auto existingIndex = findBookIndexByISBN(draft.isbn))
            {
                switch (resolveDuplicateISBN(*existingIndex, draft))
                {
                    case DuplicateResolution::CancelAdd:
                        std::cout << "\nAdd Book cancelled. Returning to Inventory Menu.\n";
                        return;
                    case DuplicateResolution::EditFields:
                        statusMessage = "Duplicate ISBN. Update fields before saving.";
                        continue;
                    case DuplicateResolution::CompletedAdd:
                        statusMessage = "Existing record updated.";
                        draft = BookDraft{};
                        continue;
                }
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

            if (draft.retail < draft.wholesale)
            {
                statusMessage = "Book added to database. Warning: Retail price is less than wholesale cost.";
            }
            else if (inventory.size() >= kMaxInventory)
            {
                statusMessage = "Book added. Inventory is now full (20/20).";
            }
            else
            {
                statusMessage = "Book added to database.";
            }

            draft = BookDraft{};
            continue;
        }

        int menuSelection = -1;
        if (!parseNonNegativeInt(choice, menuSelection) ||
            menuSelection < 1 || menuSelection > 8)
        {
            statusMessage = "Please choose an option from the menu.";
            continue;
        }

        PromptResult result = promptForField(static_cast<FieldChoice>(menuSelection), draft);
        if (result == PromptResult::Cancel)
        {
            statusMessage = "Entry cancelled.";
        }
        else
        {
            statusMessage = fieldName(static_cast<FieldChoice>(menuSelection)) + " saved.";
        }
    }
}

void editBook()
{
    std::cout << "Edit Book is not implemented yet.\n";
}

void deleteBook()
{
    std::cout << "Delete Book is not implemented yet.\n";
}
