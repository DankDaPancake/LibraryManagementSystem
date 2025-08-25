#pragma once

#include "patterns/decorator/BookDecorator.hpp"

// Decorator pattern for adding special tags to a book
class SpecialTagDecorator: public BookDecorator {
private:
    vector<string> tags;

public:
    SpecialTagDecorator(shared_ptr<Book> book, const vector<string> &tagsList);
        
    string getFullDescription() const override;
};