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