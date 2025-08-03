#pragma once

#include <string>

using std::string;

class Author {
private:
    string authorID;
    string authorName;
    string biography;

public:
    Author(const string &id, const string &name, const string &bio);

    string getAuthorID() const;
    string getName() const;
    string getBiograph() const;
};