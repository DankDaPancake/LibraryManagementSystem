#include "patterns/strategy/ISearchStrategy.hpp"
#include "core/Book.hpp"

class AuthorSearchStrategy: public ISearchStrategy {
public:
    vector<Book *> search(const vector<Book *> &books, string query) const;
};