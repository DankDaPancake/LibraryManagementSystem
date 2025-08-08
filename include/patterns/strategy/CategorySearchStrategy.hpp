#include "patterns/strategy/ISearchStrategy.hpp"

class CategorySearchStrategy: public ISearchStrategy {
public:
    vector<Book *> search(const vector<Book *> &books, const string& query) const;
};