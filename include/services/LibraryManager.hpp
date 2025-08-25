#pragma once

#include <chrono>
#include "core/Book.hpp"
#include "core/Member.hpp"
#include "core/Loan.hpp"
#include "patterns/strategy/ISearchStrategy.hpp"
#include "patterns/strategy/IPenaltyStrategy.hpp"

using namespace std;

class LibraryManager {
    friend class Librarian;
    friend class NotificationService;
    
private:
    vector<Book *> books;
    vector<Author *> authors;
    vector<Category *> categories;
    vector<Member *> members;
    vector<Loan *> loans;
    ISearchStrategy *searchStrategy;
    IPenaltyStrategy *penaltyStrategy;

    LibraryManager(): searchStrategy(nullptr), penaltyStrategy(nullptr) {}
    LibraryManager(const LibraryManager &) = delete;
    LibraryManager &operator=(const LibraryManager &) = delete;

public:
    static LibraryManager &getInstance();
    
    void setSearchStrategy(ISearchStrategy *strategy);
    void setPenaltyStrategy(IPenaltyStrategy *strategy);

    vector<Book *> searchBooks(const string &query) const;

    Member* findMember(const string &memberID) const;
    Book* findBook(const string &ISBN) const;

    bool borrowBook(const string &memberID, const string &ISBN);
    bool returnBook(const string &memberID, const string &ISBN);

    void addBookToSystem(Book *book);
    void addMemberToSystem(Member *member);

    void loadBooksIntoLibrary();
    void saveBooksNewInfo();
};  