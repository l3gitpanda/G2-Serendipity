/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 10/13/2025
  Purpose: Inventory Menu
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
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

    BookDraft createDraftFromBook(const bookType &book)
    {
        BookDraft draft;

        draft.title      = book.getTitle();
        draft.isbn       = book.getISBN();
        draft.author     = book.getAuthor();
        draft.publisher  = book.getPublisher();
        draft.dateAdded  = book.getDateAdded();
        draft.quantity   = book.getQtyOnHand();
        draft.wholesale  = book.getWholesale();
        draft.retail     = book.getRetail();

        draft.titleSet     = true;
        draft.isbnSet      = true;
        draft.authorSet    = true;
        draft.publisherSet = true;
        draft.dateSet      = true;
        draft.quantitySet  = true;
        draft.wholesaleSet = true;
        draft.retailSet    = true;

        return draft;
    }

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

    void renderBookForm(const BookDraft &draft,
                        const std::string &statusMessage,
                        const std::string &heading,
                        const std::string &saveOptionLabel)
    {
        clearScreen();

        // Frame helpers (consistent with menus/cashier/bookinfo)
        constexpr int FRAME_W = 80;
        constexpr int INSIDE  = 78;
        auto border = [&]() { std::cout << std::string(FRAME_W, '-') << '\n'; };
        auto framed = [&](const std::string &s) {
            std::string t = s;
            if ((int)t.size() < INSIDE) t += std::string(INSIDE - (int)t.size(), ' ');
            else if ((int)t.size() > INSIDE) t = t.substr(0, INSIDE);
            std::cout << '-' << t << "-\n";
        };
        auto blank = [&]() { framed(""); };

        border();
        framed("SERENDIPITY BOOKSELLERS");
        blank();
        framed(heading);
        blank();
        {
            std::ostringstream oss;
            oss << "DATABASE SIZE: " << kMaxInventory
                << " CURRENT BOOK COUNT: " << bookType::getBookCount();
            framed(oss.str());
        }
        blank();

        if (!statusMessage.empty())
        {
            framed(statusMessage);
            blank();
        }

        framed("--PENDING VALUES--");

        auto fieldLine = [&](int index, const std::string &label, const std::string &value)
        {
            std::ostringstream row;
            std::ios::fmtflags flags = row.flags();
            row << ' ' << index << ") " << std::left << std::setw(34) << label
                << "-> " << value;
            row.flags(flags);
            framed(row.str());
        };

        fieldLine(1, fieldLabel(FieldChoice::Title), draft.titleSet ? draft.title : "--EMPTY--");
        fieldLine(2, fieldLabel(FieldChoice::ISBN), draft.isbnSet ? draft.isbn : "--EMPTY--");
        fieldLine(3, fieldLabel(FieldChoice::Author), draft.authorSet ? draft.author : "--EMPTY--");
        fieldLine(4, fieldLabel(FieldChoice::Publisher), draft.publisherSet ? draft.publisher : "--EMPTY--");
        fieldLine(5, fieldLabel(FieldChoice::DateAdded), draft.dateSet ? draft.dateAdded : "--EMPTY--");
        fieldLine(6, fieldLabel(FieldChoice::Quantity), draft.quantitySet ? std::to_string(draft.quantity) : "0");
        fieldLine(7, fieldLabel(FieldChoice::Wholesale), draft.wholesaleSet ? formatMoney(draft.wholesale) : "$0.00");
        fieldLine(8, fieldLabel(FieldChoice::Retail), draft.retailSet ? formatMoney(draft.retail) : "$0.00");

        blank();
        {
            std::ostringstream save;
            save << ' ' << 9 << ") " << saveOptionLabel;
            framed(save.str());
        }
        framed(" 0) Return to Inventory Menu");
        border();
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
        // Preserve spaces in the query so a single space matches titles with spaces.
        std::string needle = toLowerCopy(query);
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

            navigationMenu header{
                "Inventory Database - Search Results",
                {}
            };
            std::cout << "DATABASE SIZE: " << kMaxInventory << '\n'
                      << "Books in Database: " << bookType::getBookCount() << "\n\n"
                      << "Results for "
                      << ((type == SearchType::Title) ? "Title" : "ISBN")
                      << " \"" << query << "\"\n\n";
            header.printWithBack("Return to Inventory Menu");

            for (std::size_t i = 0; i < matches.size(); ++i)
            {
                const bookType &book = inventory[matches[i]];
                std::cout << std::setw(2) << (i + 1) << ") "
                          << book.getTitle() << " — " << book.getAuthor()
                          << " — " << book.getISBN()
                          << " — Qty: " << book.getQtyOnHand()
                          << " — " << formatMoney(book.getRetail()) << '\n';
            }

            std::cout << "Select # to view details (0 to return): ";

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
                      << "Retail: " << formatMoney(existing.getRetail()) << "\n\n";

            navigationMenu optionsMenu{
                "Duplicate ISBN Options",
                {
                    "Edit fields before saving",
                    "Increase quantity on existing record"
                }
            };
            optionsMenu.printWithBack("Cancel add and return to Inventory Menu");
            std::cout << "Choice: ";

            std::string input;
            if (!std::getline(std::cin, input))
            {
                return DuplicateResolution::CancelAdd;
            }

            input = trim(input);
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

            if (input == "0")
            {
                return DuplicateResolution::CancelAdd;
            }

            std::cout << "Please enter 0, 2, or 3.\n";
            pressEnterToContinue();
        }
    }
} // namespace

static void printInvMenu()
{
    navigationMenu menu{
        "Inventory Menu",
        {
            "Look Up Book",
            "Add Book",
            "Edit Book",
            "Delete Book"
        }
    };
    menu.printWithBack("Return to Main Menu");
}

void invMenu()
{
    ensureInventoryCapacity();

    bool running = true;
    while (running)
    {
        clearScreen();
    printInvMenu();
    std::cout << "Choice: ";

        std::string input;
        if (!std::getline(std::cin, input))
        {
            return;
        }

        input = trim(input);
        if (input.size() != 1 ||
            (input[0] != '0' && input[0] != '1' && input[0] != '2' && input[0] != '3' && input[0] != '4'))
        {
            std::cout << "\nPlease enter 0, 1, 2, 3, or 4.\n";
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
            case '3':
                clearScreen();
                editBook();
                break;
            case '4':
                clearScreen();
                deleteBook();
                break;
            case '0':
                pressEnterToContinue();
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
        // Print submenu using navigationMenu with borders
        navigationMenu menu{
            "Inventory Database - Look Up Book",
            {
                "Search by Title",
                "Search by ISBN"
            }
        };
        // Optional context above or below the menu
        std::cout << "DATABASE SIZE: " << kMaxInventory << '\n'
                  << "Books in Database: " << bookType::getBookCount() << "\n\n";
    menu.printWithBack("Return to Inventory Menu");
    std::cout << "Choice: ";

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

        // Do NOT trim here: allow a single space (" ") to search for titles containing spaces.
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

    BookDraft draft;
    std::string statusMessage =
        (inventory.size() >= kMaxInventory)
            ? "Inventory full (" + std::to_string(kMaxInventory) + "/" +
                  std::to_string(kMaxInventory) +
                  ". You may review fields, but saving is disabled until space is available."
            : "Select a field number to enter information. Leave a field blank while editing to cancel that entry.";

    while (true)
    {
        renderBookForm(draft, statusMessage, "ADD BOOK", "Save Book to Database");
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
            pressEnterToContinue();
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
                statusMessage = "Inventory full (" + std::to_string(kMaxInventory) + "/" +
                                std::to_string(kMaxInventory) + ". Cannot add more books.";
                continue;
            }

            if (auto existingIndex = findBookIndexByISBN(draft.isbn))
            {
                switch (resolveDuplicateISBN(*existingIndex, draft))
                {
                    case DuplicateResolution::CancelAdd:
                        std::cout << "\nAdd Book cancelled. Returning to Inventory Menu.\n";
                        pressEnterToContinue();
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
                statusMessage = "Book added. Inventory is now full (" +
                                std::to_string(kMaxInventory) + "/" +
                                std::to_string(kMaxInventory) + ").";
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
    if (inventory.empty())
    {
        std::cout << "Inventory is empty. Add books before editing.\n";
        pressEnterToContinue();
        return;
    }

    int selectedIndex = lookUpBook();
    if (selectedIndex == -1)
    {
        return;
    }

    BookDraft draft = createDraftFromBook(inventory[static_cast<std::size_t>(selectedIndex)]);
    std::string statusMessage =
        "Select a field number to edit. Leave a field blank while editing to cancel that entry.";

    while (true)
    {
        renderBookForm(draft, statusMessage, "EDIT BOOK", "Save Changes");
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
            std::cout << "\nEdit Book cancelled. Returning to Inventory Menu.\n";
            pressEnterToContinue();
            return;
        }

        if (choice == "9")
        {
            if (!draft.isComplete())
            {
                statusMessage = "All fields must be completed before saving.";
                continue;
            }

            auto duplicate = findBookIndexByISBN(draft.isbn);
            if (duplicate.has_value() && static_cast<int>(*duplicate) != selectedIndex)
            {
                statusMessage = "Another book already uses this ISBN. Update the ISBN before saving.";
                continue;
            }

            bookType &book = inventory[static_cast<std::size_t>(selectedIndex)];
            book.setTitle(draft.title);
            book.setISBN(draft.isbn);
            book.setAuthor(draft.author);
            book.setPublisher(draft.publisher);
            book.setDateAdded(draft.dateAdded);
            book.setQtyOnHand(draft.quantity);
            book.setWholesale(draft.wholesale);
            book.setRetail(draft.retail);

            std::cout << "\nChanges saved.";
            if (draft.retail < draft.wholesale)
            {
                std::cout << " Warning: Retail price is less than wholesale cost.";
            }
            std::cout << '\n';
            pressEnterToContinue();
            return;
        }

        int menuSelection = -1;
        if (!parseNonNegativeInt(choice, menuSelection) || menuSelection < 1 || menuSelection > 8)
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

void deleteBook()
{
    if (inventory.empty())
    {
        std::cout << "Inventory is empty. Nothing to delete.\n";
        pressEnterToContinue();
        return;
    }

    int selectedIndex = lookUpBook();
    if (selectedIndex == -1)
    {
        return;
    }

    while (true)
    {
        clearScreen();

        const bookType &book = inventory[static_cast<std::size_t>(selectedIndex)];
        std::cout << "Serendipity Booksellers\n\n"
                  << "Delete Book\n\n"
                  << "Title : " << book.getTitle() << '\n'
                  << "Author: " << book.getAuthor() << '\n'
                  << "ISBN  : " << book.getISBN() << '\n'
                  << "Qty   : " << book.getQtyOnHand() << '\n'
                  << "Retail: " << formatMoney(book.getRetail()) << "\n\n"
                  << "Are you sure you want to delete this book? (y/n): ";

        std::string confirmation;
        if (!std::getline(std::cin, confirmation))
        {
            std::cout << "\nDelete Book cancelled.\n";
            pressEnterToContinue();
            return;
        }

        std::string normalized = toLowerCopy(trim(confirmation));
        if (normalized == "y" || normalized == "yes")
        {
            inventory.erase(inventory.begin() + selectedIndex);
            std::cout << "\nBook deleted from inventory.\n";
            pressEnterToContinue();
            return;
        }

        if (normalized == "n" || normalized == "no" || normalized.empty())
        {
            std::cout << "\nDelete Book cancelled.\n";
            pressEnterToContinue();
            return;
        }

        std::cout << "\nPlease enter 'y' or 'n'.\n";
        pressEnterToContinue();
    }
}
