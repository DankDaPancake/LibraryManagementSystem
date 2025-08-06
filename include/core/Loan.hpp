#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>

using namespace std;

using Date = chrono::system_clock::time_point;

enum class LoanStatus {
    ACTIVE,
    RETURNED,
    OVERDUE
};

class Loan {
private:
    string loanID, bookISBN, memberID;
    Date borrowDate, dueDate, returnDate;
    LoanStatus status;

public:
    string getLoanID() const;
    string getBookISBN() const;
    string getMemberID() const;
    Date getBorrowDate() const;
    Date getDueDate() const;
    Date getReturnDate() const;
    LoanStatus getStatus() const;

    void setLoanID(const string &id);
    void setBookISBN(const string &isbn);
    void setMemberID(const string &id);
    void setBorrowDate(const Date &date);
    void setDueDate(const Date &date);
    void setReturnDate(const Date &date);
    void setStatus(LoanStatus newStatus);

    bool isOverDue();
    int calculateFine() const;
    static string dateToString(const Date &date);
};