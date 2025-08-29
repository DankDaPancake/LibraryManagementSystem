#pragma once

#include "patterns/observer/ISubject.hpp"
#include "patterns/observer/IObserver.hpp"
#include "core/Book.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class BookSubject: public ISubject {
private:
    vector<IObserver*> observers;
    Book* book;  // Changed to raw pointer
    string bookStatusToString(BookStatus status);

public:
    // Constructor that takes a raw pointer to a Book object
    BookSubject(Book* associatedBook);
    // Destructor
    ~BookSubject() override;

    void attach(IObserver* observer) override;
    void detach(IObserver* observer) override;
    void notify() override;

    void setBookStatusStatusAndNotify(BookStatus newStatus);

    // Return the book pointer
    Book* getBook() const;
};