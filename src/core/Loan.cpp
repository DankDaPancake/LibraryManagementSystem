#include "core/Loan.hpp"

Loan::Loan(const string &loanId, const string &isbn, const string &memberId,
           Date borrowDate, Date dueDate, Date returnDate, LoanStatus status) 
    : loanID(loanId), bookISBN(isbn), memberID(memberId), status(status),
      borrowDate(borrowDate), dueDate(dueDate), returnDate(returnDate) {}

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
    return daysOverdue * 10000; // penalty (VND)
}

string Loan::dateToString(const Date &date) {
    time_t time = chrono::system_clock::to_time_t(date);
    tm tm = *localtime(&time);

    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

Date Loan::stringToDate(const string &dateStr) {
    if (dateStr == "") 
        return chrono::system_clock::from_time_t(0);
    tm tm = {};
    istringstream iss(dateStr);
    iss >> get_time(&tm, "%Y-%m-%d");
    if (iss.fail()) {
        throw runtime_error("Failed to parse date string: " + dateStr);
    }
    time_t time = mktime(&tm);
    return chrono::system_clock::from_time_t(time);
}

string Loan::loanStatusToString(LoanStatus status) {
    switch (status) {
        case static_cast<LoanStatus>(0):
            return "ACTIVE";
        case static_cast<LoanStatus>(1):
            return "RETURNED";
        case static_cast<LoanStatus>(2):
            return "OVERDUE";
    }
    return "UNKNOWN";
}

string Loan::loanCSVFormat() const {
    string result = loanID + "," + bookISBN + "," + memberID + "," + dateToString(borrowDate) 
            + "," + dateToString(dueDate) + "," + dateToString(returnDate) + loanStatusToString(status);
    return result;
}