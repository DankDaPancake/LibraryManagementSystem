#include "patterns/decorator/SpecialTagDecorator.hpp"

SpecialTagDecorator::SpecialTagDecorator(shared_ptr<Book> book, const vector<string> &tagsList)
    : BookDecorator(book), tags(tagsList) {}

string SpecialTagDecorator::getFullDescription() const {
    string desc = BookDecorator::getFullDescription() + ",{";
    for (size_t i = 0; i < tags.size(); i++) {
        desc += tags[i];
        if (i < tags.size() - 1) desc += ",";
    }
    desc += "}";
    return desc;
}