#include "patterns/observer/BookSubject.hpp"

string BookSubject::bookStatusToString(BookStatus status) {
    switch (status) {
        case static_cast<BookStatus>(0):
            return "AVAILABLE";
        case static_cast<BookStatus>(1):
            return "UNAVAILABLE";
    }
    return "UNKNOWN";
}

BookSubject::BookSubject(Book* associatedBook) : book(associatedBook) {}

BookSubject::~BookSubject() {
    // No need to delete book as it's managed by LibraryManager
}

void BookSubject::attach(IObserver* observer) {
    if (observer) {
        observers.push_back(observer);
    }
}

void BookSubject::detach(IObserver* observer) {
    if (observer) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }
}

void BookSubject::notify() {
    string statusMsg = bookStatusToString(book->getStatus());
    string message = "Book status updated: " + statusMsg;
    
    for (auto observer : observers) {
        if (observer) {
            observer->update(message, book, nullptr);
        }
    }
}

void BookSubject::setBookStatusStatusAndNotify(BookStatus newStatus) {
    if (book) {
        book->setStatus(newStatus);
        notify();
    }
}

Book* BookSubject::getBook() const {
    return book;
}