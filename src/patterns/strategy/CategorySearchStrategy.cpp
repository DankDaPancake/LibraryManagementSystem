#include "patterns/strategy/CategorySearchStrategy.hpp"
#include "utils/StringHandler.hpp"

vector<Book *> CategorySearchStrategy::search(const vector<Book *> &books, string query) const {
    vector<pair<string, Book *>> categories;
    for (auto &book: books) {
        categories.push_back({book->getCategory().getName(), book});
    }

    auto scores = StringHandler::findTopMatches(categories, query, categories.size());
    
    vector<Book *> results;
    for (const auto &[score, book]: scores) 
        results.push_back(book);
    return results;
}