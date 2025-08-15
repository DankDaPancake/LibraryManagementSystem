#pragma once

#include "core/Author.hpp"
#include "core/Category.hpp"
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
    Category category;
    BookStatus status;
    int totalCopies;
    int availableCopies;

public:
    Book(const string &ISBN, const string &title, 
         const string &authorID, const string &authorName, const string &biography, 
         const string &categoryID, const string &categoryName, const string &description,
         BookStatus status, int totalCopies, int availableCopies);
    virtual ~Book() = default;

    string getISBN() const;
    string getTitle() const;
    Author getAuthor() const;
    Category getCategory() const;
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