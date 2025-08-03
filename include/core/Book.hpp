#pragma once

#include <iostream>
#include <vector>

using namespace std;

enum class BookStatus {
    AVAILABLE,          
    BORROWED,           
    RESERVED,           
    UNDER_MAINTENANCE
};

class Book {
private:
    string ISBN;
    string title;
    string categoryID;
    vector<string> authorIDs;
    BookStatus status;
    int totalCopies;
    int availableCopies;

public:
    Book(const string &isbn, const string &title, const string &catergoryID,
         const vector<string> &authorIDs, int totalCopies);
    virtual ~Book();
};