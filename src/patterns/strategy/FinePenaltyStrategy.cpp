// FinePenaltyStrategy.cpp
#include "patterns/strategy/FinePenaltyStrategy.hpp"
#include "core/Member.hpp"
#include "core/Loan.hpp"
#include "patterns/observer/LoanSubject.hpp"
#include <iostream>
#include <memory>
#include <sstream>

void FinePenaltyStrategy::applyPenalty(Member* member, Loan* loan, int daysOverdue) {
    if (!member || !loan) return;
    
    // Use the existing calculateFine method from Loan
    double fineAmount = loan->calculateFine();
    member->addFine(fineAmount);
    
    // Format the fine amount with two decimal places
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << fineAmount;
    std::string fineStr = stream.str();
    
    // Create notification message
    std::string message = "FINE APPLIED: $" + fineStr + 
                          " fine for book overdue by " + std::to_string(daysOverdue) + " days.";
    
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
    
    std::cout << "Fine penalty applied to member " << member->getUserName() 
              << "\". Fine amount: $" << fineStr 
              << ". Total outstanding fines: $" << member->getTotalFines() << std::endl;
}