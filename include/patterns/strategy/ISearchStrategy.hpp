#pragma once

#include <vector>
#include <string>
#include "core/Book.hpp"

using namespace std;

class ISearchStrategy {
public:
    virtual vector<Book *> search(const vector<Book *> &books, const string& query) const = 0;
    virtual ~ISearchStrategy() = default;
};