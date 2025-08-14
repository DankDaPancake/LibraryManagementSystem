#include "core/Category.hpp"

Category::Category(const string &categoryID, const string &name, const string &description)
    : categoryID(categoryID), name(name), description(description) {}

string Category::getCategoryID() const { return categoryID; }
string Category::getName() const { return name; }
string Category::getDescription() const  { return description; }