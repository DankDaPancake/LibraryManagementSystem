#include "core/Author.hpp"

Author::Author(const string &id, const string &name, const string &bio) {
    authorID = id;
    authorName = name;
    biography = bio;
}

string Author::getAuthorID() const { return authorID; }
string Author::getName() const { return authorName; }
string Author::getBiograph() const { return biography; }