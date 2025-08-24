#include "patterns\decorator\AdditionalInfoDecorator.hpp"

AdditionalInfoDecorator::AdditionalInfoDecorator(shared_ptr<Book> book, const string &info)
    : BookDecorator(book), detailedInfo(info) {}


string AdditionalInfoDecorator::getFullDescription() const {
    return BookDecorator::getFullDescription() + "," + detailedInfo;
}