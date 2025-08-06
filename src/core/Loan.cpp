#include "core/Loan.hpp"

// Getters
string Loan::getLoanID() const { return loanID; }
string Loan::getBookISBN() const { return bookISBN; }
string Loan::getMemberID() const { return memberID; }
Date Loan::getBorrowDate() const { return borrowDate; }
Date Loan::getDueDate() const { return dueDate; }
Date Loan::getReturnDate() const { return returnDate; }
LoanStatus Loan::getStatus() const { return status; }

// Setters
void Loan::setLoanID(const string &id) { loanID = id; }
void Loan::setBookISBN(const string &isbn) { bookISBN = isbn; }
void Loan::setMemberID(const string &id) { memberID = id; }
void Loan::setBorrowDate(const Date &date) { borrowDate = date; }
void Loan::setDueDate(const Date &date) { dueDate = date; }
void Loan::setReturnDate(const Date &date) { returnDate = date; }
void Loan::setStatus(LoanStatus newStatus) { status = newStatus; }

bool Loan::isOverDue() {
    auto now = chrono::system_clock::now();
    if (status == LoanStatus::ACTIVE && now > dueDate)  
        status = LoanStatus::OVERDUE;
    return (status == LoanStatus::OVERDUE);
}

int Loan::calculateFine() const {
    if (status != LoanStatus::OVERDUE)
        return 0;
    
    int daysOverdue = chrono::duration_cast<chrono::hours>(returnDate - dueDate).count() / 24;
    return daysOverdue * 10000; // penalty
}

string Loan::dateToString(const Date &date) {
    time_t time = chrono::system_clock::to_time_t(date);
    tm tm = *localtime(&time);

    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}