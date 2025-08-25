#pragma once

#include "patterns/observer/ISubject.hpp"
#include "patterns/observer/IObserver.hpp"
#include "core/Book.hpp"

#include <algorithm>
#include <iostream>
// just for debugging purposes
#include <string>
#include <memory>
#include <vector>

using namespace std;

class BookSubject: public ISubject {
private:
    vector<IObserver *> observers;
    shared_ptr<Book> book;
    string bookStatusToString(BookStatus status);

public:
    // Constructor that takes a shared pointer to a Book object
    BookSubject(shared_ptr<Book> associatedBook);
    // Destructor
    ~BookSubject() override;

    void attach(IObserver *observer) override;
    void detach(IObserver *observer) override;
    void notify() override;

    void setBookStatusStatusAndNotify(BookStatus newStatus);

    // Set the book's total copies and notify observers
    shared_ptr<Book> getBook() const;
};
