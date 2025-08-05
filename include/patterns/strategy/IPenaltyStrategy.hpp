#pragma once

#include <string>
#include "core/Member.hpp"
#include "core/Loan.hpp"

class IPenaltyStrategy {
public:
    virtual void applyPenalty(Member* member, Loan* loan, int daysOverdue) const = 0;
    virtual ~IPenaltyStrategy() = default;
};

class FinePenaltyStrategy: public IPenaltyStrategy {
public:
    void applyPenalty(Member* member, Loan* loan, int daysOverdue);
};

class WarningPenaltyStrategy: public IPenaltyStrategy {
public:
    void applyPenalty(Member* member, Loan* loan, int daysOverdue);
};

class SuspendPenaltyStrategy: public IPenaltyStrategy {
public:
    void applyPenalty(Member* member, Loan* loan, int daysOverdue);
};
