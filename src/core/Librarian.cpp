#include "core/Librarian.hpp"

Librarian::Librarian(const string &userID, const string &userName, const string &password)
         : User(userID, userName, password, Role::LIBRARIAN) {}

void Librarian::addBook(Book *book) {
    if (!book) {
        cout << "Error adding book: Book pointer cannot be null." << endl;
        return;
    }

    LibraryManager &libraryManager = LibraryManager::getInstance();

    if (book->getISBN().empty()) {
        cout << "Error adding book: Book ISBN cannot be empty." << endl;
        return;
    }

    if (book->getTitle().empty()) {
        cout << "Error adding book: Book title cannot be empty." << endl;
        return;
    }

    Book *existingBook = nullptr;
    for (auto libraryBook: libraryManager.books) {
        if (libraryBook->getISBN() == book->getISBN()) {
            existingBook = libraryBook;
            break;
        }
    }

    if (existingBook) {
        existingBook->setTotalCopies(existingBook->getTotalCopies() + book->getTotalCopies());
        existingBook->setAvailableCopies(existingBook->getAvailableCopies() + book->getAvailableCopies());

        cout << "Book with ISBN " << book->getISBN() << " already exists.";
        cout << "Update to " << existingBook->getTotalCopies() << " total copies." << endl;
    }
    else {
        libraryManager.books.push_back(book);
        cout << "New book '" << book->getTitle() << "' added successfully." << endl;
    }
}

void Librarian::removeBook(const string &ISBN) {
    if (ISBN.empty()) {
        cout << "Error removing book: ISBN cannot be empty." << endl;
        return;
    }

    LibraryManager &libraryManager = LibraryManager::getInstance();

    Book *existingBook = nullptr;
    for (auto libraryBook: libraryManager.books) {
        if (libraryBook->getISBN() == ISBN) {
            existingBook = libraryBook;
            break;
        }
    }

    if (!existingBook) {
        cout << "Error removing book: ISBN " << ISBN << " not found." << endl;
        return;
    }

    bool hasActiveLoans = false;
    for (auto libraryLoan: libraryManager.loans) {
        if (libraryLoan->getBookISBN() == ISBN && libraryLoan->getStatus() == LoanStatus::ACTIVE) {
            hasActiveLoans = true;
            break;
        }
    }

    if (hasActiveLoans) {
        cout << "Error removing book: Cannot remove book '" << existingBook->getTitle()
             << "'. It has active loan(s)." << endl;
        return;
    }

    string title = existingBook->getTitle();
    libraryManager.books.erase(
        remove(libraryManager.books.begin(), libraryManager.books.end(), existingBook),
        libraryManager.books.end()
    );

    delete existingBook;
    cout << "Book '" << title << "' removed successfully." << endl;
}

void Librarian::updateBookInfo(Book *book) {
    if (!book) {
        cout << "Error updating book: Book pointer cannot be null." << endl;
        return;
    }

    LibraryManager &libraryManager = LibraryManager::getInstance();

    if (book->getISBN().empty()) {
        cout << "Error updating book: ISBN cannot be empty." << endl;
        return;
    }

    if (book->getTitle().empty()) {
        cout << "Error updating book: Title cannot be empty." << endl;
        return;
    }

    Book *existingBook = nullptr;
    for (auto libraryBook: libraryManager.books) {
        if (libraryBook->getISBN() == book->getISBN()) {
            existingBook = libraryBook;
            break;
        }
    }

    if (!existingBook) {
        cout << "Error updating book: ISBN " << book->getISBN() << "' not found." << endl;
        return;
    }

    existingBook->setTotalCopies(book->getTotalCopies());

    int currentBorrowed = existingBook->getTotalCopies() - existingBook->getAvailableCopies();
    int newAvailable = book->getTotalCopies() - currentBorrowed;
    existingBook->setAvailableCopies(max(0, newAvailable));
    
    cout << "Book information updated successfully!" << endl;
    cout << "Title: " << existingBook->getTitle() << endl;
    cout << "Total copies: " << existingBook->getTotalCopies() << endl;
    cout << "Available copies: " << existingBook->getAvailableCopies() << endl;
}

void Librarian::manageMemberInfo(Member *member) {
    if (!member) {
        cout << "Error managing member: Pointer cannot be null." << endl;
        return;
    }

    LibraryManager &libraryManager = LibraryManager::getInstance();

    cout << endl << "=== MANAGING MEMBER ===" << endl;
    cout << "Member ID: " << member->getUserID() << endl;
    cout << "Username: " << member->getUserName() << endl;
    // need ot move dateToString() method somewhere else
    cout << "Membership date: " << Loan::dateToString(member->getMembershipDate()) << endl;

    vector<Book *> borrowedBooks = member->getBorrowedBooks();
    cout << "Currently borrow books: " << borrowedBooks.size() << endl;
    for (size_t i = 0; i < borrowedBooks.size(); i++) {
        cout << "  " << i + 1 << ". " << borrowedBooks[i]->getTitle()
             << " (ISBN: " << borrowedBooks[i]->getISBN() << ")" << endl;
    }

    vector<Loan *> memberLoans;
    for (const auto &loan: libraryManager.loans) {
        if (loan->getMemberID() == member->getUserID()) {
            memberLoans.push_back(loan);
        }
    }

    int activeLoans = 0, returnedLoans = 0, overdueLoans = 0;
    cout << endl << "Loan History (" << memberLoans.size() << " loans):" << endl;

    string statusStr;
    for (const auto &loan: memberLoans) {
        switch (loan->getStatus()) {
            case LoanStatus::RETURNED:
                statusStr = "Returned";
                returnedLoans++;
                break;

            default:
                statusStr = "Active";
                activeLoans++;
                if (loan->isOverDue()) {
                    overdueLoans++;
                    statusStr += " (OVERDUE)";
                }
                break;
        }

        cout << "  - ISBN: " << loan->getBookISBN() << ", Loan status: " << statusStr;
        if (loan->getStatus() != LoanStatus::RETURNED) {
            int fine = loan->calculateFine();
            if (fine > 0) 
                cout << ", Fine: " << fine << " VND";
        }
        cout << endl;
    }

    cout << endl << "Summary: " << endl;
    cout << "  Active loans: " << activeLoans << endl;
    cout << "  Overdue loans: " << overdueLoans << endl;
    cout << "  Returned loans: " << returnedLoans << endl;

    if (overdueLoans > 0) {
        cout << endl << "*** WARNING: This member has overdue loans! ***" << endl;
    }

    // Add managing options ....
} 

vector<Loan *> Librarian::viewAllLoans() {
    LibraryManager &libraryManager = LibraryManager::getInstance();

    cout << endl << "=== ALL LOANS REPORT ===" << endl;
    cout << "Total loans: " << libraryManager.loans.size() << endl;

    if (libraryManager.loans.empty()) {
        cout << "No loans found in the system." << endl;
        return libraryManager.loans;
    }

    int activeLoans = 0, returnedLoans = 0, overdueLoans = 0;
    int totalFines = 0;

    for (const auto &loan: libraryManager.loans) {
        switch (loan->getStatus()) {
            case LoanStatus::RETURNED:
                returnedLoans++;
                break;

            default:
                activeLoans++;
                if (loan->isOverDue()) {
                    overdueLoans++;
                    totalFines += loan->calculateFine();
                }
                break;
        }
    }

    cout << endl << "=== DETAILED LOAN LIST ===" << endl;
    cout << left << setw(12) << "Loan ID" 
                 << setw(12) << "Member ID"
                 << setw(15) << "ISBN" 
                 << setw(12) << "Status" 
                 << setw(10) << "Fine" << endl;
    cout << string(61, '-') << endl;

    string statusStr;
    for (const auto &loan: libraryManager.loans) {
        switch (loan->getStatus()) {
            case LoanStatus::RETURNED:
                statusStr = "Returned";
                returnedLoans++;
                break;

            default:
                statusStr = "Active";
                activeLoans++;
                if (loan->isOverDue()) {
                    overdueLoans++;
                    statusStr += " (OVERDUE)";
                }
                break;
        }
        cout << left << setw(12) << loan->getLoanID()
             << setw(12) << loan->getMemberID()
             << setw(15) << loan->getBookISBN()
             << setw(12) << statusStr
             << loan->calculateFine() << " VND" << endl;
    }

    return libraryManager.loans;
}