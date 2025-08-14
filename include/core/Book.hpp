#pragma once

#include "core/Author.hpp"
#include <iostream>
#include <vector>

using namespace std;

enum class BookStatus {
    AVAILABLE = 0,          
    UNAVAILABLE = 1
};

class Book {
private:
    string ISBN;
    string title;
    Author author;
    string categoryID;
    BookStatus status;
    int totalCopies;
    int availableCopies;

public:
    Book(const string &ISBN, const string &title, const string &athorName, 
         const string &authorID, const string &categoryID, BookStatus status,
         int totalCopies, int availableCopies);
    virtual ~Book() = default;

    string getISBN() const;
    string getTitle() const;
    string getAuthorName() const;
    string getAuthorID() const;
    string getCategoryID() const;
    BookStatus getStatus() const;
    int getTotalCopies() const;
    int getAvailableCopies() const;

    void setStatus(BookStatus newStatus);
    void setTotalCopies(int count);
    void setAvailableCopies(int count);

    string bookStatusToString() const;
    bool isAvailable() const;

    void displayBasicInfo() const; // for terminal output
    string getCSVDescription() const; // fetch as CSV - for GUI
};