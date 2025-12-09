/*
CS1B – G2: Serendipity
  Partner A: Ian Bagherzadeh — role: InvMenu AddBook & bookinfo
  Partner B: Ryan Posey — role: lookUpBook
  Date: 11/16/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp bookType.cpp cashier.cpp bookinfo.cpp -o serendipity.out
*/

#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "invmenu.h"
#include "reports.h"
#include "utils.h"
using namespace std;

static void printReportsMenu()
{
    navigationMenu menu{
        "Reports",
        {
            "Inventory Listing",
            "Inventory Wholesale Value",
            "Inventory Retail Value",
            "Listing by Quantity",
            "Listing by Cost",
            "Listing by Age"
        }
    };
    menu.printWithBack("Return to Main Menu");
}

void reports()
{
    bool running = true;

    while (running)
    {
        clearScreen();
        printReportsMenu();
        cout << "Enter Your Choice: ";

        string input;
        getline(cin, input);

        input = trim(input);
        int selection = -1;
        if (!parseNonNegativeInt(input, selection))
        {
            cout << "\nPlease enter a number in the range 1 - 7.\n";
            pressEnterToContinue();
            continue;
        }

        const int returnIndex = 6 + 1; // 6 options + return
        if (selection == returnIndex)
        {
            pressEnterToContinue();
            running = false;
            continue;
        }

        switch (selection)
        {
            case 1:
                clearScreen();
                repListing();
                pressEnterToContinue();
                break;
            case 2:
                clearScreen();
                repWholesale();
                pressEnterToContinue();
                break;
            case 3:
                clearScreen();
                repRetail();
                pressEnterToContinue();
                break;
            case 4:
                clearScreen();
                repQty();
                pressEnterToContinue();
                break;
            case 5:
                clearScreen();
                repCost();
                pressEnterToContinue();
                break;
            case 6:
                clearScreen();
                repAge();
                pressEnterToContinue();
                break;
            default:
                cout << "\nPlease enter a number in the range 1 - 7.\n";
                pressEnterToContinue();
                break;
        }
    }
}

namespace
{
    struct Column
    {
        std::string heading;
        std::size_t width;
    };

    std::string truncateField(const std::string &value, std::size_t width)
    {
        if (value.size() <= width)
        {
            return value;
        }

        if (width <= 3)
        {
            return value.substr(0, width);
        }

        return value.substr(0, width - 3) + "...";
    }

    void printReportHeader(std::size_t capacity)
    {
        std::cout << "Serendipity Booksellers\n";
        std::cout << "Inventory Listing\n";
        std::cout << "Database Capacity: " << capacity << '\n';
        std::cout << "Books in Database: " << bookType::recordCount() << "\n\n";
    }

    template <typename Iterator>
    void reportListingImpl(Iterator begin, Iterator end, std::size_t capacity)
    {
        const std::vector<Column> columns = {
            {"TITLE", 30},
            {"ISBN", 13},
            {"AUTHOR", 18},
            {"PUBLISHER", 18},
            {"DATE ADDED", 12},
            {"QTY O/H", 8},
            {"WHOLESALE", 11},
            {"RETAIL", 11},
        };

        auto printColumnHeadings = [&]() {
            for (const auto &col : columns)
            {
                std::cout << std::left << std::setw(static_cast<int>(col.width)) << col.heading << ' ';
            }
            std::cout << "\n";
            for (const auto &col : columns)
            {
                std::cout << std::string(col.width, '-') << ' ';
            }
            std::cout << "\n";
        };

        const auto total = static_cast<std::size_t>(std::distance(begin, end));
        if (total == 0)
        {
            printReportHeader(capacity);
            printColumnHeadings();
            std::cout << "(no books to display)\n";
            return;
        }

        Iterator current = begin;
        while (current != end)
        {
            printReportHeader(capacity);
            printColumnHeadings();

            std::size_t linesThisPage = 0;
            while (current != end && linesThisPage < 10)
            {
                const bookType *book = *current;
                ++current;
                ++linesThisPage;
                std::cout << std::left
                          << std::setw(30) << truncateField(book->getTitle(), 30) << ' '
                          << std::setw(13) << truncateField(book->getISBN(), 13) << ' '
                          << std::setw(18) << truncateField(book->getAuthor(), 18) << ' '
                          << std::setw(18) << truncateField(book->getPublisher(), 18) << ' '
                          << std::setw(12) << truncateField(book->getDateAdded(), 12) << ' '
                          << std::right << std::setw(8) << book->getQtyOnHand() << ' '
                          << std::right << std::setw(11) << formatMoney(book->getWholesale()) << ' '
                          << std::right << std::setw(11) << formatMoney(book->getRetail()) << '\n';
                std::cout << std::left; // reset alignment for text columns
            }

            if (current != end)
            {
                std::cout << "\nPress any key to continue ...";
                std::cout.flush();
                std::cin.get();
                clearScreen();
            }
        }
    }
} // namespace

void repListing()
{
    reportListing(inventoryData(), inventoryCapacity());
}

void reportListing(const std::vector<bookType *> &books, std::size_t capacity)
{
    reportListingImpl(books.begin(), books.end(), capacity);
}

void reportListing(bookType *const *books, std::size_t count, std::size_t capacity)
{
    reportListingImpl(books, books + count, capacity);
}

// ----- Report stubs -----
void repWholesale() { cout << "Inventory Wholesale Value selected.\n"; }
void repRetail()    { cout << "Inventory Retail Value selected.\n"; }
void repQty()       { cout << "Listing by Quantity selected.\n"; }
void repCost()      { cout << "Listing by Cost selected.\n"; }
void repAge()       { cout << "Listing by Age selected.\n"; }