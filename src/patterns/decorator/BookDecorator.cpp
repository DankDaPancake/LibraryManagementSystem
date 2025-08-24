#include "patterns\decorator\BookDecorator.hpp"

BookDecorator::BookDecorator(shared_ptr<Book> book) : Book(book->getISBN(), book->getTitle(),
    book->getAuthor().getAuthorID(), book->getAuthor().getName(), book->getAuthor().getBiograph(),
    book->getCategory().getCategoryID(), book->getCategory().getName(), book->getCategory().getDescription(),
    book->getStatus(), book->getTotalCopies(), book->getAvailableCopies()), decoratedBook(book) {}

string BookDecorator::getFullDescription() const {
    // Call the getFullDescription method of the decorated book
    return decoratedBook->getFullDescription();
}