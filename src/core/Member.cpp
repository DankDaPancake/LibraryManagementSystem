#include "core/Member.hpp"

Member::Member(const string &userID, const string &userName, const string &password, Role role = Role::MEMBER)
      : User(userID, userName, password, role) {
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
    // Interact with library manager
}

bool Member::returnBook(const string &ISBN) {
    
}