#include "patterns\observer\LoanSubject.hpp"

string LoanSubject::loanStatusToString(LoanStatus status) {
    switch (status) {
    case LoanStatus::ACTIVE:
        return "ACTIVE";
    case LoanStatus::OVERDUE:
        return "OVERDUE";
    case LoanStatus::RETURNED:
        return "RETURNED";
    }
    return "UNKNOWN";
}

LoanSubject::LoanSubject(Loan* associatedLoan) : loan(associatedLoan) {}

LoanSubject::~LoanSubject() {
    // No need to delete loan as it's managed by LibraryManager
}

void LoanSubject::attach(IObserver* observer) {
    if (observer) {
        observers.push_back(observer);
    }
}

void LoanSubject::detach(IObserver* observer) {
    if (observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }
}

void LoanSubject::notify() {
    string statusMsg = loanStatusToString(loan->getStatus());
    string message = "Loan status updated: " + statusMsg;
    
    // Add more context depending on the status
    if (loan->getStatus() == LoanStatus::OVERDUE) {
        int daysOverdue = loan->getDaysOverdue();
        message += " - Loan is overdue by " + to_string(daysOverdue) + " days";
    } else if (loan->getStatus() == LoanStatus::RETURNED) {
        message += " - Book has been successfully returned";
    }
    
    for (auto observer : observers) {
        if (observer) {
            observer->update(message, nullptr, loan);
        }
    }
}

void LoanSubject::setLoanStatusAndNotify(LoanStatus newStatus) {
    if (loan) {
        loan->setStatus(newStatus);
        notify();
    }
}

Loan* LoanSubject::getLoan() const {
    return loan;
}