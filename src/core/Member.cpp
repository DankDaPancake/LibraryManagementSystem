#include "core/Member.hpp"

#include "services/LibraryManager.hpp"

Member::Member(const string &userID, const string &userName, const string &password)
      : User(userID, userName, password, Role::MEMBER) {
    membershipDate = chrono::system_clock::now();    
}

Member::~Member() {
    for (Book *book: borrowedBooks) {
        if (book && book->getTitle() == "Unknown Title") {
            delete book;
        }
    }
}

Date Member::getMembershipDate() const { return membershipDate; }
vector<Book *> Member::getBorrowedBooks() const { return borrowedBooks; }

bool Member::borrowBook(const string &ISBN) {
    LibraryManager &libraryManager = LibraryManager::getInstance();
    return libraryManager.borrowBook(getUserID(), ISBN);
}

bool Member::returnBook(const string &ISBN) {
    LibraryManager &libraryManager = LibraryManager::getInstance();
    return libraryManager.returnBook(getUserID(), ISBN);
}

// Methods for penalty handling
void Member::addWarning() {
    ++warningCount;
}

int Member::getWarningCount() const {
    return warningCount;
}

void Member::resetWarnings() {
    warningCount = 0;
}

void Member::suspendAccount(int days) {
    suspended = true;
    suspensionEndDate = chrono::system_clock::to_time_t(chrono::system_clock::now() + chrono::hours(24 * days));
}

bool Member::isSuspended() const {
    return suspended && chrono::system_clock::to_time_t(chrono::system_clock::now()) < suspensionEndDate;
}

void Member::removeSuspension() {
    suspended = false;
    suspensionEndDate = 0;
}

void Member::addFine(double amount) {
    totalFines += amount;
}

double Member::getTotalFines() const {
    return totalFines;
}

void Member::payFine(double amount) {
    totalFines -= amount;
    if (totalFines < 0) {
        totalFines = 0;
    }
}