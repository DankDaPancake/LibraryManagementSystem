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

BookSubject::BookSubject(shared_ptr<Book> associatedBook) 
    : book(associatedBook) {}

BookSubject::~BookSubject() {
    // destructor for BookSubject no needed delete pointers
    // because they are managed by the observers themselves
}

void BookSubject::attach(IObserver *observer)
{
    observers.push_back(observer);
}

void BookSubject::detach(IObserver *observer)
{
    observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
}

void BookSubject::notify()
{
    string message = "Book status updated: " + bookStatusToString(book->getStatus());
    for (IObserver *observer : observers)
    {
        if (observer)
        {
            observer->update(message, book.get(), nullptr);
        }
        // Example debug log: cout << "Notification sent by BookSubject for Book: " << book->getTitle() << endl;
    }
}

void BookSubject::setBookStatusStatusAndNotify(BookStatus newStatus)
{
    book->setStatus(newStatus);
    notify();
}

shared_ptr<Book> BookSubject::getBook() const
{
    return book;
}