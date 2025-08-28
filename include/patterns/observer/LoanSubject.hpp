#pragma once

#include "patterns/observer/ISubject.hpp"
#include "patterns/observer/IObserver.hpp"
#include "core/Loan.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

class LoanSubject: public ISubject {
private:
    vector<IObserver *> observers;
    shared_ptr<Loan> loan;
    string loanStatusToString(LoanStatus status);

public:
    // Constructor that takes a shared pointer to a Loan object
    LoanSubject(shared_ptr<Loan> associatedLoan);
    // Destructor
    ~LoanSubject() override;

    void attach(IObserver *observer) override;
    void detach(IObserver *observer) override;
    void notify() override;

    void setLoanStatusAndNotify(LoanStatus newStatus);
    
    // Get the associated loan
    shared_ptr<Loan> getLoan() const;
};