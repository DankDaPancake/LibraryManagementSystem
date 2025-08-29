#pragma once

#include "patterns/observer/ISubject.hpp"
#include "patterns/observer/IObserver.hpp"
#include "core/Loan.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class LoanSubject: public ISubject {
private:
    vector<IObserver*> observers;
    Loan* loan;  // Changed to raw pointer
    string loanStatusToString(LoanStatus status);

public:
    // Constructor that takes a raw pointer to a Loan object
    LoanSubject(Loan* associatedLoan);
    // Destructor
    ~LoanSubject() override;

    void attach(IObserver* observer) override;
    void detach(IObserver* observer) override;
    void notify() override;

    void setLoanStatusAndNotify(LoanStatus newStatus);
    
    // Get the associated loan
    Loan* getLoan() const;
};