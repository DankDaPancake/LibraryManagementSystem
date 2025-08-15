#include "patterns/strategy/AuthorSearchStrategy.hpp"
#include "utils/StringHandler.hpp"

vector<Book *> AuthorSearchStrategy::search(const vector<Book *> &books, string query) const {
    vector<pair<string, Book *>> authors;
    for (auto &book: books) {
        authors.push_back({book->getAuthor().getName(), book});
    }

    auto scores = StringHandler::findTopMatches(authors, query, 10);
    
    vector<Book *> results;
    for (const auto &[score, book]: scores) 
        results.push_back(book);
    return results;
}