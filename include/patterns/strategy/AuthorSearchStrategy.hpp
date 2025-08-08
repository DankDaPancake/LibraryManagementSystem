#include "patterns/strategy/ISearchStrategy.hpp"

class AuthorSearchStrategy: public ISearchStrategy {
public:
    vector<Book *> search(const vector<Book *> &books, const string& query) const;
};