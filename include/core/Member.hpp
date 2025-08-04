#pragma once

#include <vector>
#include <chrono>

#include "core/User.hpp"
#include "core/Book.hpp"

using namespace std;
using Date = chrono::system_clock::time_point;

class Member: public User {
private:
    Date membershipDate;
    vector<Book *> borrowedBooks;

public:
    Member(const string &userID, const string &userName, const string &password, Role role = Role::MEMBER);
    ~Member();

    Date getMembershipDate() const;
    vector<Book *> getBorrowedBooks() const;

    bool borrowBook(const string &ISBN);
    bool returnBook(const string &ISBN);
};