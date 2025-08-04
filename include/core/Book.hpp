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
    Book(const string &isbn, const string &title, const string &categoryID,
         const vector<string> &authorIDs, int totalCopies);
    virtual ~Book() = default;

    string getISBN() const;
    string getTitle() const;
    string getCategoryID() const;
    const vector<string> &getAuthorIDs() const;
    BookStatus getStatus() const;
    int getTotalCopies() const;
    int getAvailableCopies() const;

    void setStatus(BookStatus newStatus);
    void setTotalCopies(int count);
    void setAvailableCopies(int count);

    string bookStatusToString() const;
    bool isAvailable() const;

    void displayBasicInfo() const; // for terminal output
    string getFullDescription() const; // fetch as CSV - for GUI
};