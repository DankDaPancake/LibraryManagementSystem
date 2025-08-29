#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>

using namespace std;

using Date = chrono::system_clock::time_point;

enum class LoanStatus {
    ACTIVE = 0,
    RETURNED = 1,
    OVERDUE = 2
};

class Loan {
private:
    string loanID, bookISBN, memberID;
    Date borrowDate, dueDate, returnDate;
    LoanStatus status;

public:
    Loan() = default;
    Loan(const string &loanId, const string &isbn, const string &memberId,
         Date borrowDate, Date dueDate, Date returnDate, LoanStatus status);

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

    bool isOverdue();
    int getDaysOverdue() const;
    int calculateFine() const;
    string loanCSVFormat() const;
    
    static string dateToString(const Date &date);
    static Date stringToDate(const string &dateStr);
    static string loanStatusToString(LoanStatus status);
};