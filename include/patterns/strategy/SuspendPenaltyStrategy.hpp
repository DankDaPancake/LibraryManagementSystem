#include "patterns/strategy/IPenaltyStrategy.hpp"

class SuspendPenaltyStrategy: public IPenaltyStrategy {
public:
    void applyPenalty(Member* member, Loan* loan, int daysOverdue);
};
