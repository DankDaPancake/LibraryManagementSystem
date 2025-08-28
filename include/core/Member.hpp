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

    // Penalty-related attributes
    int warningCount = 0;
    bool suspended = false;
    time_t suspensionEndDate = 0;
    double totalFines = 0.0;

public:
    Member(const string &userID, const string &userName, const string &password);
    ~Member();

    Date getMembershipDate() const;
    vector<Book *> getBorrowedBooks() const;

    bool borrowBook(const string &ISBN);
    bool returnBook(const string &ISBN);

    // Methods for penalty handling
    void addWarning();
    int getWarningCount() const;
    void resetWarnings();
    
    void suspendAccount(int days);
    bool isSuspended() const;
    void removeSuspension();
    
    void addFine(double amount);
    double getTotalFines() const;
    void payFine(double amount);
};