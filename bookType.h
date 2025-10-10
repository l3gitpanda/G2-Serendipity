/*
CS1B – G2: Serendipity
  Partner A: Ryan Posey — role: driver (mainmenu.cpp)
  Partner B: Ian Bagherzadeh — role: Inventory Manager (invMenu.cpp/.h)
  Date: 9/23/2025
  Purpose: Menu based inventory manager
  Build:   g++ -std=c++20 -Werror mainmenu.cpp utils.cpp invmenu.cpp reports.cpp book.cpp cashier.cpp -o serendipity.out
*/
#ifndef BOOKTYPE_H
#define BOOKTYPE_H
#include <iostream>

using namespace std;

class bookType {
    string date;
    unsigned int quantity; // Must be positive
    string ISBN;
    string title;
    float price;
    

    static size_t totalBookCount;

    
    
    public:
    
    static bool validateDateInput(string date);
    static string sanitizeDateInput(string date);
};



#endif // BOOKTYPE_H