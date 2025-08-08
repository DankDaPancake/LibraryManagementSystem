#include "patterns/strategy/ISearchStrategy.hpp"

class TitleSearchStrategy: public ISearchStrategy {
public:
    vector<Book *> search(const vector<Book *> &books, const string& query) const;
};