#pragma once

#include <memory>
#include "core/Book.hpp"

// Decorator pattern for extending the functionality of Book class
class BookDecorator: public Book {
protected:
    // Pointer to the book being deccorated
    shared_ptr<Book> decoratedBook;

public:
    BookDecorator(shared_ptr<Book> book);

    string getFullDescription() const override;
};