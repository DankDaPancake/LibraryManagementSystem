#pragma once

#include "patterns/decorator/BookDecorator.hpp"

// Decorator pattern for adding difficulty label to a book
class DifficultyLabelDecorator : public BookDecorator {
private:
    string difficultyLabel;

public:
    DifficultyLabelDecorator(shared_ptr<Book> book, const string &label)
        : BookDecorator(book), difficultyLabel(label) {}
    
    string getFullDescription() const override;
};