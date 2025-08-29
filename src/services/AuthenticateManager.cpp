#include "services/AuthenticateManager.hpp"
#include "services/LibraryManager.hpp"
#include "core/Member.hpp"
#include "patterns/observer/MemberObserver.hpp"
#include "core/Librarian.hpp"
#include "patterns/observer/LibrarianObserver.hpp"

bool AuthenticateManager::registerUser(const string &userName, const string &password, Role role) {
    if (password.size() < 8) {
        cout << "Registration failed. Password must contain at least 8 characters." << endl;
        return false;
    }
    string fileName = (role == Role::LIBRARIAN
                        ? "../data/librarians.csv"
                        : "../data/members.csv");
    string prefix = (role == Role::LIBRARIAN? "L": "M");

    string newUserID = CSVHandler::generateUserID(prefix);

    if (CSVHandler::addUser(newUserID, userName, password, fileName)) {
        cout << "Registration successful! Your User ID: " << newUserID << endl;
        return true;
    }
    else {
        cout << "Registration failed. Please try again." << endl;
        return false;
    }
}

User* AuthenticateManager::loginUser(const string &userName, const string &password) {
    string userID = CSVHandler::validateCredentials(userName, password, "../data/librarians.csv");
    LibraryManager &libManager = LibraryManager::getInstance();
    if (userID[0] == 'L') {
        Librarian *librarian = new Librarian(userID, userName, password);
        libManager.setObserver(new LibrarianObserver(librarian));
        libManager.addObserverToAllBooks();
        libManager.addObserverToAllLoans();
        return new User(userID, userName, password, Role::LIBRARIAN);
    }

    userID = CSVHandler::validateCredentials(userName, password, "../data/members.csv");
    if (userID[0] == 'M') {
        Member *member = new Member(userID, userName, password);
        libManager.setObserver(new MemberObserver(member));
        libManager.addObserverToAllBooks();
        libManager.addObserverToAllLoans();
        return new User(userID, userName, password, Role::MEMBER);
    }

    cout << "Invalid credentials. Please try again." << endl;
    return nullptr;
}

void AuthenticateManager::logoutUser() {
    LibraryManager &libManager = LibraryManager::getInstance();
    libManager.removeObserverBooks();
    libManager.removeObserverLoans(); 
    libManager.setObserver(nullptr);
    cout << "Successfully logged out." << endl;
}