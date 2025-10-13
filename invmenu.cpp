#include "invmenu.h"

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

    enum class PromptResult
    {
        Success,
        Cancel
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

    void renderAddBookForm(const BookDraft &draft)
    {
        clearScreen();

        std::size_t currentCount = inventory.size();
        std::cout << "Serendipity Booksellers\n\n"
                  << "Inventory Database - Add Book\n\n"
                  << "DATABASE SIZE: " << kMaxInventory << '\n'
                  << "Books in Database: " << bookType::getBookCount() << "\n"
                  << "Add Book (" << (currentCount + 1) << '/' << kMaxInventory << ")\n\n";

        auto fieldLine = [](int index, const std::string &label, const std::string &value)
        {
            std::cout << std::setw(2) << index << ") " << label;
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

        std::cout << "\n 9) Save Book to Database\n"
                  << " 0) Cancel Add Book\n";
    }

    PromptResult promptString(const std::string &prompt, std::string &value)
    {
        for (;;)
        {
            std::cout << prompt << " (press ENTER to cancel): ";

            std::string input;
            if (!std::getline(std::cin, input))
            {
                return PromptResult::Cancel;
            }

            std::string trimmed = trim(input);
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
            std::cout << prompt << " (press ENTER to cancel): ";

            std::string input;
            if (!std::getline(std::cin, input))
            {
                return PromptResult::Cancel;
            }

            std::string trimmed = trim(input);
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
            std::cout << prompt << " (press ENTER to cancel): ";

            std::string input;
            if (!std::getline(std::cin, input))
            {
                return PromptResult::Cancel;
            }

            std::string trimmed = trim(input);
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
            std::cout << prompt << " (press ENTER to cancel): ";

            std::string input;
            if (!std::getline(std::cin, input))
            {
                return PromptResult::Cancel;
            }

            std::string trimmed = trim(input);
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

    std::vector<int> searchInventory(const std::string &query)
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
            std::string titleLower  = toLowerCopy(book.getTitle());
            std::string authorLower = toLowerCopy(book.getAuthor());
            std::string isbnLower   = toLowerCopy(book.getISBN());

            if (titleLower.find(needle) != std::string::npos ||
                authorLower.find(needle) != std::string::npos ||
                isbnLower.find(needle) != std::string::npos)
            {
                matches.push_back(static_cast<int>(index));
            }
        }

        return matches;
    }

    int showSearchResults(const std::vector<int> &matches, const std::string &query)
    {
        while (true)
        {
            clearScreen();

            std::cout << "Serendipity Booksellers\n\n"
                      << "Inventory Database - Search Results\n\n"
                      << "DATABASE SIZE: " << kMaxInventory << '\n'
                      << "Books in Database: " << bookType::getBookCount() << "\n\n"
                      << "Results for \"" << query << "\"\n\n";

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
                pressEnterToContinue();
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
                  << "Enter title, author, or ISBN (press ENTER to cancel): ";

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

        std::vector<int> matches = searchInventory(query);
        if (matches.empty())
        {
            std::cout << "\nNo matches found.\n";
            pressEnterToContinue();
            clearScreen();
            continue;
        }

        return showSearchResults(matches, query);
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
              << "Books in Database: " << bookType::getBookCount() << "\n\n"
              << "Press ENTER at any prompt to cancel.\n\n";

    for (int option = static_cast<int>(FieldChoice::Title);
         option <= static_cast<int>(FieldChoice::Retail); ++option)
    {
        PromptResult result = promptForField(static_cast<FieldChoice>(option), draft);
        if (result == PromptResult::Cancel)
        {
            std::cout << "\nAdd Book cancelled. Returning to Inventory Menu.\n";
            return;
        }
    }

    bool editing = true;
    while (editing)
    {
        renderAddBookForm(draft);
        std::cout << "\nSelect a field number to edit, 9 to save, or 0 to cancel: ";

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

            if (auto existingIndex = findBookIndexByISBN(draft.isbn))
            {
                switch (resolveDuplicateISBN(*existingIndex, draft))
                {
                    case DuplicateResolution::CancelAdd:
                        std::cout << "\nAdd Book cancelled. Returning to Inventory Menu.\n";
                        return;
                    case DuplicateResolution::EditFields:
                        continue;
                    case DuplicateResolution::CompletedAdd:
                        if (draft.retail < draft.wholesale)
                        {
                            std::cout << "Warning: Retail price is less than wholesale cost.\n";
                        }
                        return;
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

            std::size_t count = inventory.size();
            if (count == kMaxInventory)
            {
                std::cout << "\nBook added. Inventory is now full (" << count << '/' << kMaxInventory << ").\n";
            }
            else
            {
                std::cout << "\nBook added successfully. Books in Database: "
                          << count << '/' << kMaxInventory << "\n";
            }

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

        PromptResult result = promptForField(static_cast<FieldChoice>(menuSelection), draft);
        if (result == PromptResult::Cancel)
        {
            std::cout << "\nAdd Book cancelled. Returning to Inventory Menu.\n";
            return;
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
