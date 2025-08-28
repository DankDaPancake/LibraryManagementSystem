// SuspendPenaltyStrategy.cpp
#include "patterns/strategy/SuspendPenaltyStrategy.hpp"
#include "core/Member.hpp"
#include "core/Loan.hpp"
#include "patterns/observer/LoanSubject.hpp"
#include <iostream>
#include <memory>

void SuspendPenaltyStrategy::applyPenalty(Member* member, Loan* loan, int daysOverdue) {
    if (!member || !loan) return;
    
    // Suspend for a period proportional to days overdue (e.g., 2x the days overdue)
    int suspensionDays = daysOverdue * 2;
    member->suspendAccount(suspensionDays);
    
    // Create notification message
    std::string message = "ACCOUNT SUSPENDED: Member account suspended for " + 
                          std::to_string(suspensionDays) + " days due to overdue book.";
    
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
    
    std::cout << "Suspension penalty applied to member " << member->getUserName() 
              << "\". Suspended for " << suspensionDays << " days." << std::endl;
}