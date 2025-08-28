// WarningPenaltyStrategy.cpp
#include "patterns/strategy/WarningPenaltyStrategy.hpp"
#include "core/Member.hpp"
#include "core/Loan.hpp"
#include "patterns/observer/LoanSubject.hpp"
#include <iostream>
#include <memory>

void WarningPenaltyStrategy::applyPenalty(Member* member, Loan* loan, int daysOverdue) {
    if (!member || !loan) return;
    
    member->addWarning();
    
    // Create notification message
    std::string message = "WARNING: Book overdue by " + std::to_string(daysOverdue) + 
                          " days. This is warning #" + std::to_string(member->getWarningCount());
    
    // Use LoanSubject to notify observers
    std::shared_ptr<Loan> sharedLoan(loan, [](Loan* l) {}); // Non-owning shared_ptr
    LoanSubject loanSubject(sharedLoan);
    
    // Update loan status to OVERDUE if not already
    if (loan->getStatus() != LoanStatus::OVERDUE) {
        loanSubject.setLoanStatusAndNotify(LoanStatus::OVERDUE);
    } else {
        // Just notify without changing status
        loanSubject.notify();
    }
    
    std::cout << "Warning penalty applied to member " << member->getUserName()     
              << "\". Warning count: " << member->getWarningCount() << std::endl;
}