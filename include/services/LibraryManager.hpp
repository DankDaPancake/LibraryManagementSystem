#pragma once

#include <iostream>
#include <fstream>
#include <sstream>    
#include <algorithm>  
#include <chrono>    
#include <cctype>
#include <vector>
#include <thread> 
#include <atomic>
#include <mutex>
#include "core/Book.hpp"
#include "core/Member.hpp"
#include "core/Loan.hpp"
#include "patterns/strategy/ISearchStrategy.hpp"
#include "patterns/strategy/IPenaltyStrategy.hpp"
#include "patterns/observer/LoanSubject.hpp"
#include "patterns/observer/BookSubject.hpp"

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
    vector<BookSubject *> bookSubjects;
    vector<LoanSubject *> loanSubjects;

    ISearchStrategy *searchStrategy;
    IPenaltyStrategy *penaltyStrategy;
    IObserver *observer;

    thread timerThread;
    atomic<bool> stopTimer;
    mutex loansMutex;

    LibraryManager(): stopTimer(true), searchStrategy(nullptr), penaltyStrategy(nullptr) {}
    LibraryManager(const LibraryManager &) = delete;
    LibraryManager &operator=(const LibraryManager &) = delete;

    void loanCheckTimer();
    void checkLoansAndApplyPenalties();
    IPenaltyStrategy* selectPenaltyStrategy(int daysOverdue);

public:
    static LibraryManager &getInstance();
    
    void setSearchStrategy(ISearchStrategy *strategy);
    void setObserver(IObserver *observer);

    vector<Book *> searchBooks(const string &query) const;

    Member* findMember(const string &memberID) const;
    Book* findBook(const string &ISBN) const;
    const std::vector<Book*>& getBooks() const;
    const std::vector<Loan*>& getLoans() const;
    const std::vector<Member*>& getMembers() const;

    bool borrowBook(const string &memberID, const string &ISBN);
    bool returnBook(const string &memberID, const string &ISBN);
    bool addBook(const std::string& ISBN, const std::string& title, int authorID, int categoryID, int totalCopies);

    void addBookToSystem(Book *book);
    void addMemberToSystem(Member *member);

    void loadBooksIntoLibrary();
    void saveBooksNewInfo();

    void loadMembersFromCSV(const string& path = "../data/members.csv");
    void loadLoansFromCSV(const string& path = "../data/loans.csv");

    void startLoanCheckTimer();
    void stopLoanCheckTimer();

    void addObserverToAllBooks();
    void addObserverToAllLoans();
    void removeObserverBooks();
    void removeObserverLoans();

    void systemLogout();
};  