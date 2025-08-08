#include "patterns/strategy/IPenaltyStrategy.hpp"

class WarningPenaltyStrategy: public IPenaltyStrategy {
public:
    void applyPenalty(Member* member, Loan* loan, int daysOverdue);
};