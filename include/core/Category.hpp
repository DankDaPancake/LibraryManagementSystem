#pragma once

#include <string>

using namespace std;

class Category {
private:
    string categoryID;
    string name;
    string description;

public:
    Category(const string &categoryID, const string &name, const string &description);

    string getCategoryID() const;
    string getName() const;
    string getDescription() const;
};