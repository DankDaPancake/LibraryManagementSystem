#include "core/Book.hpp"

Book::Book(const string &ISBN, const string &title, 
           const string &authorID, const string &authorName, const string &biography, 
           const string &categoryID, const string &categoryName, const string &description,
           BookStatus status, int totalCopies, int availableCopies)
    : ISBN(ISBN), title(title), author(authorID, authorName, biography),
      category(categoryID, categoryName, description), status(status),
      totalCopies(totalCopies), availableCopies(availableCopies) {}

string Book::getISBN() const { return ISBN; }
string Book::getTitle() const { return title; }
Author Book::getAuthor() const { return author; }
Category Book::getCategory() const { return category; }
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
    cout << "   + Category ID: " << category.getCategoryID() << endl;
    cout << "   + Status: " << bookStatusToString() << endl;
    cout << "   + Copies: " << availableCopies << "/" << totalCopies << " available" << endl;
}

string Book::getFullDescription() const {
    string desc = ISBN + "," + title + "," + author.getAuthorID() + "," +
                  category.getCategoryID() + "," + bookStatusToString() + "," +
                  to_string(totalCopies) + "," + to_string(availableCopies);
    return desc;
}
