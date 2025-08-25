#pragma once

#include "patterns/observer/IObserver.hpp"
#include "core/Member.hpp"

#include <fstream>
#include <string>

using namespace std;

class LibrarianObserver: public IObserver {
private:
    Member *member;

public:
    LibrarianObserver(Member *member) : member(member) {}

    void update(const string &message, const Book *book, Loan *loan) override;

    ~LibrarianObserver() override = default;

    void logNotification(const string &message, const Book *book, Loan *loan) override;
};