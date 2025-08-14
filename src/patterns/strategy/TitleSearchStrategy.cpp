#include "patterns/strategy/TitleSearchStrategy.hpp"
#include "utils/StringHandler.hpp"

vector<Book *> TitleSearchStrategy::search(const vector<Book *> &books, string query) const {
    vector<pair<string, Book *>> titles;
    for (auto &book: books) {
        titles.push_back({book->getTitle(), book});
    }

    auto scores = StringHandler::findTopMatches(titles, query, 10);
    
    vector<Book *> results;
    for (const auto &[score, book]: scores) 
        results.push_back(book);
    return results;
}