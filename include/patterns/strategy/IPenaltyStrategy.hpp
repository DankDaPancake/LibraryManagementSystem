#pragma once

#include <string>
#include "core/Member.hpp"
#include "core/Loan.hpp"

class IPenaltyStrategy {
public:
    virtual void applyPenalty(Member* member, Loan* loan, int daysOverdue) = 0;
    virtual ~IPenaltyStrategy() = default;
};