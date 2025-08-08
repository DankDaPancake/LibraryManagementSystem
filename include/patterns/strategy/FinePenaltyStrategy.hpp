#include "patterns/strategy/IPenaltyStrategy.hpp"

class FinePenaltyStrategy: public IPenaltyStrategy {
public:
    void applyPenalty(Member* member, Loan* loan, int daysOverdue);
};