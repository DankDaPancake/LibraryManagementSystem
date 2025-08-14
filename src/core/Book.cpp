#include "core/Book.hpp"

Book::Book(const string &ISBN, const string &title, const string &authorName,
           const string &authorID, const string &categoryID, BookStatus status,
           int totalCopies, int availableCopies)
    : ISBN(ISBN), title(title), author(authorID, authorName, ""),
      categoryID(categoryID), totalCopies(totalCopies), availableCopies(totalCopies)
{}

string Book::getISBN() const { return ISBN; }
string Book::getTitle() const { return title; }
string Book::getAuthorName() const { return author.getName(); }
string Book::getAuthorID() const { return author.getAuthorID(); }
string Book::getCategoryID() const { return categoryID; }
BookStatus Book::getStatus() const { return status; }
int Book::getTotalCopies() const { return totalCopies; }
int Book::getAvailableCopies() const { return availableCopies; }

void Book::setStatus(BookStatus newStatus) {
    status = newStatus;
}

void Book::setTotalCopies(int count) {
    if (count >= 0) {
        totalCopies = count;
        if (availableCopies > totalCopies)
            availableCopies = totalCopies;
        if (availableCopies == 0)
            status = static_cast<BookStatus>(1);
    }
}

void Book::setAvailableCopies(int count) {
    if (0 <= count && count <= totalCopies) {
        availableCopies = count;
        if (availableCopies == 0)
            status = static_cast<BookStatus>(1);
    }
}

string Book::bookStatusToString() const {
    switch (status) {
        case static_cast<BookStatus>(0):
            return "AVAILABLE";
        case static_cast<BookStatus>(1):
            return "UNAVAILABLE";
    }
    return "UNKNOWN";
}

bool Book::isAvailable() const {
    return status == static_cast<BookStatus>(0);
}

void Book::displayBasicInfo() const {
    cout << "   + ISBN: " << ISBN << endl;
    cout << "   + Title: " << title << endl;
    cout << "   + Author name: " << author.getName() << endl;
    cout << "   + Author IDs " << author.getAuthorID() << endl;
    cout << endl;
    cout << "   + Category ID: " << categoryID << endl;
    cout << "   + Status: " << bookStatusToString() << endl;
    cout << "   + Copies: " << availableCopies << "/" << totalCopies << " available" << endl;
}

string Book::getCSVDescription() const {
    string desc = ISBN + "," + title + "," +
                  author.getName() + "," + author.getAuthorID() + "," +
                  categoryID + "," + bookStatusToString() + "," +
                  to_string(totalCopies) + "," +
                  to_string(availableCopies);
    return desc;
}
