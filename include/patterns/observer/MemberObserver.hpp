#pragma once

#include "patterns/observer/IObserver.hpp"
#include "core/Member.hpp"

#include <fstream>
#include <string>

using namespace std;

class MemberObserver: public IObserver {
private:
    Member *member;

public:
    MemberObserver(Member *member) : member(member) {}

    void update(const string &message, const Book *book, Loan *loan) override;

    ~MemberObserver() override = default;

    void logNotification(const string &message, const Book *book, Loan *loan) override;
};