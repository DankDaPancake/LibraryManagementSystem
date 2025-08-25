#include "patterns\observer\LoanSubject.hpp"

string LoanSubject::loanStatusToString(LoanStatus status)
{
    switch (status)
    {
    case LoanStatus::ACTIVE:
        return "ACTIVE";
    case LoanStatus::OVERDUE:
        return "OVERDUE";
    case LoanStatus::RETURNED:
        return "RETURNED";
    default:
        return "UNKNOWN";
    }
}

LoanSubject::LoanSubject(shared_ptr<Loan> associatedLoan) : loan(associatedLoan)
{
    // Empty constructor body
}

LoanSubject::~LoanSubject()
{
    // Destructor for LoanSubject does not need to delete pointers
    // because they are managed by the observers themselves
}

void LoanSubject::attach(IObserver *observer)
{
    observers.push_back(observer);
}

void LoanSubject::detach(IObserver *observer)
{
    observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
}

void LoanSubject::notify()
{
    string message = "Loan status updated: " + loanStatusToString(loan->getStatus());
    for (IObserver *observer : observers)
    {
        if (observer)
        {
            observer->update(message, nullptr, loan.get());
        }
        // Example debug log: cout << "Notification sent by LoanSubject for Loan ID: " << loan->getLoanID() << endl;
    }
}

void LoanSubject::setLoanStatusAndNotify(LoanStatus newStatus)
{
    loan->setStatus(newStatus);
    notify();
}

shared_ptr<Loan> LoanSubject::getLoan() const
{
    return loan;
}