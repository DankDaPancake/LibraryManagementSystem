#include "services/LibraryManager.hpp"

LibraryManager &LibraryManager::getInstance() {
    static LibraryManager instance;
    return instance;
}

void LibraryManager::setSearchStrategy(ISearchStrategy *strategy) {
    searchStrategy = strategy;
}

void LibraryManager::setPenaltyStrategy(IPenaltyStrategy *strategy) {
    penaltyStrategy = strategy;
}

vector<Book *> LibraryManager::searchBooks(const string &query) const {
    if (searchStrategy)
        return searchStrategy->search(books, query);

    return {};
}

Member* LibraryManager::findMember(const string &memberID) const {
    Member *targetMember = nullptr;
    for (auto &member: members) {
        if (member->getUserID() == memberID) {
            targetMember = member;
            break;
        }
    }

    if (!targetMember) {
        cout << "Cannot find member with ID " << memberID << " in system." << endl;
        return nullptr;
    }
    return targetMember;
}

Book* LibraryManager::findBook(const string &ISBN) const {
    Book *targetBook = nullptr;
    for (auto &book: books) {
        if (book->getISBN() == ISBN) {
            targetBook = book;
            break;
        }
    }

    if (!targetBook) {
        cout << "Cannot find book with ISBN " << ISBN << " in library." << endl;
        return nullptr;
    }
    return targetBook;
}

bool LibraryManager::borrowBook(const string &memberID, const string &ISBN) {
    Member *targetMember = findMember(memberID);
    Book *targetBook = findBook(ISBN);

    if (targetMember == nullptr || targetBook == nullptr) 
        return false;
    if (!targetBook->isAvailable()) {
        cout << "Book with ISBN " << ISBN << " is not available for borrowing." << endl;
        return false;
    }

    auto borrowedBooks = targetMember->getBorrowedBooks();
    for (auto &book: borrowedBooks) {
        if (book->getISBN() == ISBN) {
            cout << "Member already has this book borrowed." << endl;
            return false;
        }
    }

    Loan *newLoan = new Loan();
    newLoan->setLoanID("LOAN#" + to_string(loans.size() + 1));
    newLoan->setBookISBN(ISBN);
    newLoan->setMemberID(memberID);

    auto borrowDate = chrono::system_clock::now();
    newLoan->setBorrowDate(borrowDate);
    newLoan->setDueDate(borrowDate + chrono::hours(24 * 14));
    newLoan->setStatus(LoanStatus::ACTIVE);
    loans.push_back(newLoan);

    targetBook->setAvailableCopies(targetBook->getAvailableCopies() - 1);

    cout << "Successfully borrowed book with ISBN " << ISBN << endl;
    return true;
}

bool LibraryManager::returnBook(const string &memberID, const string &ISBN) {
    Member *targetMember = findMember(memberID);
    Book *targetBook = findBook(ISBN);

    bool hasBook = false;
    auto borrowedBooks = targetMember->getBorrowedBooks();
    for (auto &book: borrowedBooks) {
        if (book->getISBN() == ISBN) {
            hasBook = true;
            break;
        }
    }

    if (!hasBook) {
        cout << "Member does not have this book borrowed." << endl;
        return false;
    }

    for (auto &loan: loans) {
        if (loan->getMemberID() == memberID && loan->getBookISBN() == ISBN) {
            loan->setReturnDate(chrono::system_clock::now());
            loan->setStatus(LoanStatus::RETURNED);
            break;
        }
    }

    targetBook->setAvailableCopies(targetBook->getAvailableCopies() + 1);

    cout << "Successfully returned book with ISBN " << endl;
    return true;
}

void LibraryManager::addBookToSystem(Book *book) {
    if (book) {
        books.push_back(book);
        cout << "Added book with ISBN " << book->getISBN() << " to library." << endl;
    }
}

void LibraryManager::addMemberToSystem(Member *member) {
    if (member) {
        members.push_back(member);
        cout << "Added member with ID " << member->getUserID() << " to system." << endl;
    }
}