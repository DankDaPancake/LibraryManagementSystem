#pragma once
#include "patterns/decorator/BookDecorator.hpp"

class AdditionalInfoDecorator : public BookDecorator {
private:
    string detailedInfo;
public:
    AdditionalInfoDecorator(shared_ptr<Book> book, const string &info);

    string getFullDescription() const override;
};