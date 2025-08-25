#pragma once

#include "patterns/decorator/BookDecorator.hpp"

// Decorator pattern for adding difficulty label to a book
class DifficultyLabelDecorator : public BookDecorator {
private:
    string difficultyLabel;

public:
    DifficultyLabelDecorator(shared_ptr<Book> book, const string &label);
    
    string getFullDescription() const override;
};