#include "services/LibraryManager.hpp"
#include "utils/CSVHandler.hpp"
#include <iostream>
#include <fstream>

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

Member *LibraryManager::findMember(const string &memberID) const {
    Member *targetMember = nullptr;
    for (auto &member : members) {
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

Book *LibraryManager::findBook(const string &ISBN) const {
    Book *targetBook = nullptr;
    for (auto &book : books) {
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

bool LibraryManager::borrowBook(const std::string& memberID, const std::string& ISBN) {
    Member* m = findMember(memberID);
    Book*   b = findBook(ISBN);
    if (!m || !b) return false;

    if (b->getAvailableCopies() <= 0) {
        std::cout << "Book with ISBN " << ISBN << " is not available for borrowing.\n";
        return false;
    }

    for (auto* bk : m->getBorrowedBooks()) {
        if (bk && bk->getISBN() == ISBN) {
            std::cout << "Member already has this book borrowed.\n";
            return false;
        }
    }

    auto* newLoan = new Loan();
    newLoan->setLoanID("LOAN" + std::to_string(loans.size() + 1));
    newLoan->setBookISBN(ISBN);
    newLoan->setMemberID(memberID);

    auto now = std::chrono::system_clock::now();
    newLoan->setBorrowDate(now);
    newLoan->setDueDate(now + std::chrono::hours(24 * 14));
    newLoan->setStatus(LoanStatus::ACTIVE);

    loans.push_back(newLoan);

    b->setAvailableCopies(b->getAvailableCopies() - 1);
    if (b->getAvailableCopies() == 0) {
        b->setStatus(BookStatus::UNAVAILABLE);
    }
    m->returnBook(b->getISBN());

    std::cout << "Successfully borrowed '" << b->getTitle() << "'.\n";
    return true;
}


bool LibraryManager::returnBook(const std::string& memberID, const std::string& ISBN) {
    Member* m = findMember(memberID);
    Book*   b = findBook(ISBN);
    if (!m || !b) return false;

    auto it = std::find_if(loans.begin(), loans.end(), [&](Loan* L){
        return L && L->getMemberID() == memberID
                 && L->getBookISBN() == ISBN
                 && L->getStatus() == LoanStatus::ACTIVE;
    });
    if (it == loans.end()) {
        std::cout << "No active loan for member " << memberID << " & ISBN " << ISBN << ".\n";
        return false;
    }

    Loan* ln = *it;
    ln->setReturnDate(std::chrono::system_clock::now());
    ln->setStatus(LoanStatus::RETURNED);

    b->setAvailableCopies(b->getAvailableCopies() + 1);
    if (b->getAvailableCopies() > 0) {
        b->setStatus(BookStatus::AVAILABLE);
    }
    m->returnBook(b->getISBN());

    std::cout << "Successfully returned '" << b->getTitle() << "'.\n";
    return true;
}

void LibraryManager::addBookToSystem(Book *book) {
    if (book) {
        books.push_back(book);
        cout << "Added book '" << book->getTitle() << "' (ISBN " << book->getISBN() << ") to library." << endl;
    }
}

void LibraryManager::addMemberToSystem(Member *member) {
    if (member) {
        members.push_back(member);
        cout << "Added member with ID " << member->getUserID() << " to system." << endl;
    }
}

void LibraryManager::loadBooksIntoLibrary() {
    string line;
    
    ifstream authorFile("../data/authors.csv");
    getline(authorFile, line);
    
    int countAuthors = 0;
    while (getline(authorFile, line)) {
        auto items = CSVHandler::parseCSVLine(line);
        string authorID = items[0], authorName = items[1], biography = items[2];
        authors.push_back(new Author(to_string(++countAuthors), authorName, biography));
        cout << "Added author '" << authorName << "' to library." << endl;
    }
    authorFile.close();

    ifstream categoryFile("../data/categories.csv");
    getline(categoryFile, line);

    int countCategories = 0;
    while (getline(categoryFile, line)) {
        auto items = CSVHandler::parseCSVLine(line);
        string categoryID = items[0], categoryName = items[1], description = items[2];
        categories.push_back(new Category(to_string(++countCategories), categoryName, description));
        cout << "Added category '" << categoryName << "' to library." << endl;
    }
    categoryFile.close();

    ifstream file("../data/books.csv");
    getline(file, line);

    while (getline(file, line)) {
        auto items = CSVHandler::parseCSVLine(line);

        string ISBN = items[0], title = items[1];
        
        int authorID, categoryID;
        stringstream ss;
        ss.str(items[2]);
        ss >> authorID;

        ss.str(items[3]);
        ss.clear();
        ss >> categoryID;

        BookStatus status = (items[4] == "AVAILABLE"? static_cast<BookStatus>(0): static_cast<BookStatus>(1));

        int totalCopies, availableCopies;
        ss.str(items[5]);
        ss.clear();
        ss >> totalCopies;
        
        ss.str(items[6]);
        ss.clear();
        ss >> availableCopies;

        addBookToSystem(new Book(ISBN, title, 
            to_string(authorID), authors[authorID - 1]->getName(), authors[authorID - 1]->getBiograph(),
            to_string(categoryID), categories[categoryID - 1]->getName(), categories[categoryID - 1]->getDescription(),
            status, totalCopies, availableCopies));
    }
    file.close();

    ifstream loanFile("../data/loans.csv");
    getline(loanFile, line);

    while (getline(loanFile, line)) {
        auto items = CSVHandler::parseCSVLine(line);
        string loanId = items[0], isbn = items[1], memberId = items[2];
        Date borrowDate = Loan::stringToDate(items[3]), dueDate = Loan::stringToDate(items[4]), returnDate = Loan::stringToDate(items[5]);
        LoanStatus loanStatus = static_cast<LoanStatus>(0);
        if (items[6] == "RETURNED") loanStatus = static_cast<LoanStatus>(1);
        if (items[6] == "OVERDUE") loanStatus = static_cast<LoanStatus>(2);

        loans.push_back(new Loan(loanId, isbn, memberId, borrowDate, dueDate, returnDate, loanStatus));
        cout << "Added '" << loanId << "' to system." << endl;
    }
    loanFile.close();
}

void LibraryManager::saveBooksNewInfo() {
    ofstream authorFile("../data/authors.csv");
    authorFile.clear();
    authorFile << "authorName,biography\n";

    for (auto author: authors) {
        cout << "Saved author '" << author->getName() << "' information."<< endl;
        authorFile << author->getAuthorID() << "," 
                   << author->getName() << "," 
                   << author->getBiograph() << '\n';
    }
    authorFile.close();

    ofstream categoryFile("../data/categories.csv");
    categoryFile.clear();
    categoryFile << "categoryName,description\n";

    for (auto category: categories) {
        cout << "Saved '" << category->getName() << "' information." << endl;
        categoryFile << category->getCategoryID() << "," 
                     << category->getName() << "," 
                     << category->getDescription() << '\n';
    }
    categoryFile.close();

    ofstream file("../data/books.csv");
    file.clear();
    file << "isbn,title,authorID,categoryID,status,totalCopies,availableCopies\n";

    for (auto book: books) {
        cout << "Saved '" << book->getTitle() << "' information." << endl;
        file << book->getFullDescription() << '\n';
    }
    file.close();

    ofstream loanFile("../data/loans.csv");
    loanFile.clear();
    loanFile << "loanID,isbn,memberID,borrowDate,dueDate,returnDate,loanstatus\n";

    for (auto loan: loans) {
        cout << "Saved '" << loan->getLoanID() << "' information." << endl;
        loanFile << loan->loanCSVFormat() << '\n';
    }
    loanFile.close();
}

void LibraryManager::loadMembersFromCSV(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Error opening file: " << path << '\n';
        return;
    }

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        auto items = CSVHandler::parseCSVLine(line); // id, name, password
        if (items.size() < 3) continue;

        if (findMember(items[0])) continue;

        auto* m = new Member(items[0], items[1], items[2]);   // <-- chỉ 3 tham số
        addMemberToSystem(m);
    }
    file.close();
}

static inline void trim_inplace(std::string& s) {
    auto issp = [](unsigned char c){ return std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
}
static inline LoanStatus parseLoanStatus(std::string s) {
    trim_inplace(s);
    for (auto& ch : s) ch = std::toupper((unsigned char)ch);
    if (s == "ACTIVE")   return LoanStatus::ACTIVE;
    if (s == "RETURNED") return LoanStatus::RETURNED;
    if (s == "OVERDUE")  return LoanStatus::OVERDUE;
    return LoanStatus::ACTIVE;
}

void LibraryManager::loadLoansFromCSV(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs.is_open()) { std::cout << "Error opening " << path << "\n"; return; }

    std::string line;
    if (!std::getline(ifs, line)) { return; } // skip header

    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        auto items = CSVHandler::parseCSVLine(line);
        if (items.size() < 7) continue;

        for (auto& s : items) trim_inplace(s);

        const std::string& loanId   = items[0];
        const std::string& isbn     = items[1];
        const std::string& memberId = items[2];
        Date borrowDate = Loan::stringToDate(items[3]);
        Date dueDate    = Loan::stringToDate(items[4]);
        Date returnDate = Loan::stringToDate(items[5]);
        LoanStatus st   = parseLoanStatus(items[6]);

        // tránh add trùng
        bool exists = std::any_of(loans.begin(), loans.end(),
                         [&](Loan* L){ return L && L->getLoanID()==loanId; });
        if (exists) continue;

        loans.push_back(new Loan(loanId, isbn, memberId, borrowDate, dueDate, returnDate, st));
    }
}