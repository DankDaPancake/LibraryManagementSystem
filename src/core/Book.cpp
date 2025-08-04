#include "core/Book.hpp"

Book::Book(const string &isbn, const string &title, const string &categoryID,
           const vector<string> &authorIDs, int totalCopies)
    : ISBN(isbn), title(title), categoryID(categoryID), authorIDs(authorIDs), 
      totalCopies(totalCopies), availableCopies(totalCopies) {
    if (totalCopies > 0)
        status = BookStatus::AVAILABLE;
    else 
        status = BookStatus::UNDER_MAINTENANCE;
}

string Book::getISBN() const { return ISBN; }
string Book::getTitle() const { return title; }
string Book::getCategoryID() const { return categoryID; }
const vector<string> &Book::getAuthorIDs() const { return authorIDs; }
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
            status = BookStatus::BORROWED;
    }
}

void Book::setAvailableCopies(int count) {
    if (0 <= count && count <= totalCopies) {
        availableCopies = count;
        if (availableCopies == 0)
            status = BookStatus::BORROWED;
    }
}

string Book::bookStatusToString() const {
    switch (status) {
        case BookStatus::AVAILABLE:
            return "AVAILABLE";
        case BookStatus::BORROWED:
            return "BORROWED";
        case BookStatus::RESERVED:
            return "RESERVED";
        case BookStatus::UNDER_MAINTENANCE:
            return "UNDER_MAINTENANCE";
        default:
            return "UNKNOWN";
    }
}

bool Book::isAvailable() const {
    return status == BookStatus::AVAILABLE;
}

void Book::displayBasicInfo() const {
    cout << "+ ISBN: " << ISBN << endl;
    cout << "+ Title: " << title << endl;
    cout << "+ Author IDs: ";
    for (size_t i = 0; i < authorIDs.size(); ++i)
        cout << authorIDs[i] << (i == authorIDs.size() - 1 ? "" : ", ");
    cout << endl;
    cout << "+ Category ID: " << categoryID << endl;
    cout << "+ Status: " << bookStatusToString() << endl;
    cout << "+ Copies: " << availableCopies << "/" << totalCopies << " available" << endl;
}

string Book::getFullDescription() const {
    string description = ISBN + ", " + title + ", " + "{";
    
    int authorsCount = authorIDs.size();
    for (int i = 0; i < authorsCount; i++) {
        description += authorIDs[i];
        if (i < authorsCount - 1) description += ", ";
    }
    description += "}, " + categoryID + ", " + bookStatusToString() +
                   ", " + to_string(totalCopies) +
                   ", " + to_string(availableCopies);   

    return description;
}
