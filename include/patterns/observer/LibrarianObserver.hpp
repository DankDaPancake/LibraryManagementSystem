#pragma once

#include "patterns/observer/IObserver.hpp"
#include "core/Librarian.hpp"

#include <fstream>
#include <string>

using namespace std;

class LibrarianObserver: public IObserver {
private:
    Librarian *librarian;

public:
    LibrarianObserver(Librarian *librarian) : librarian(librarian) {}

    void update(const string &message, const Book *book, Loan *loan) override;

    ~LibrarianObserver() override = default;

    void logNotification(const string &message, const Book *book, Loan *loan) override;
};