#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "core/Author.hpp"
#include "core/Book.hpp"
#include "core/Librarian.hpp"
#include "core/Loan.hpp"
#include "core/User.hpp"

#include "services/AuthenticateManager.hpp"
#include "services/LibraryManager.hpp"
#include "utils/CSVHandler.hpp"

using namespace std;

void ensureDataDirectoryExists() {
    filesystem::path dataDir("../data");

    if (!filesystem::exists(dataDir)) {
        cout << "Data directory '" << "../data" << "' not found. Creating..." << endl;
        try {
            if (filesystem::create_directory(dataDir)) {
                cout << "Data directory created successfully." << endl;
            }
            else {
                cout << "Error: Could not create data directory." << endl;
            }
        }
        catch (const exception &ex) {
            cout << "Error creating data directory: " << ex.what() << endl;
        }
    }
}

void execRegister(AuthenticateManager &authManager) {
    cout << endl << "=== USER REGISTRATION ===" << endl;
    string userName, password;
    cout << "Enter username: ";
    cin >> userName;
    cout << "Enter password: ";
    cin >> password;
    if (authManager.registerUser(userName, password, Role::MEMBER)) {
        cout << "Registration successful!" << endl;
    }
    else {
        cout << "Registration failed!" << endl;
    }
}

void execBookBorrowing(Member *member) {
    string ISBN;
    cout << "Enter ISBN: ";
    cin >> ISBN;
    member->borrowBook(ISBN);
}

void execBookReturning(Member *member) {
    string ISBN;
    cout << "Enter ISBN: ";
    cin >> ISBN;
    member->returnBook(ISBN);
}

void execViewingBorrowed(Member *member) {
    auto borrowedBooks = member->getBorrowedBooks();
    cout << "Your borrowed books: " << endl;
    for (size_t i = 0; i < borrowedBooks.size(); i++) {
        cout << "== [Book #" << i + 1 << "] ==" << endl;
        borrowedBooks[i]->displayBasicInfo();
    }
}

void memberTests(User *user, string password) {
    cout << endl << "=== [Member Testing section] ===" << endl;
    user->viewProfile();
    cout << "-----------------------------------" << endl;

    LibraryManager &libManager = LibraryManager::getInstance();
    
    Member *member = new Member(user->getUserID(), user->getUserName(), password);

    cout << "Adding member to library system..." << endl;
    libManager.addMemberToSystem(member);
    cout << endl;

    int choice = -1;
    do {
        cout << "---------------------------" << endl;
        cout << "1. View profile" << endl;
        cout << "2. Borrow book (ISBN)" << endl;
        cout << "3. Return book (ISBN)" << endl;
        cout << "4. View borrowed books" << endl;
        cout << "0. Log out" << endl;
        cout << "Enter your option (0-3): ";
        cin >> choice;

        switch (choice) {
            case 1:
                member->viewProfile();
                break;
            case 2:
                execBookBorrowing(member);
                break;
            case 3:
                execBookReturning(member);
                break;
            case 4:
                execViewingBorrowed(member);
                break;
        }
    } while (choice != 0);
    cout << "~ Successfully logged out." << endl;
}

void execLogin(AuthenticateManager &authManager) {
    string userName, password;
    cout << "Enter username: ";
    cin >> userName;
    cout << "Enter password: ";
    cin >> password;
    User *user = authManager.loginUser(userName, password);

    if (user) {
        cout << "Login successful! Welcome, " << user->getUserName() << endl;

        if (user->getRole() == Role::MEMBER) {
            memberTests(user, password);
        }
    }
    else {
        cout << "Login failed! Please check your credentials again." << endl;
    }
}

void printMenu() {
    LibraryManager &libManager = LibraryManager::getInstance();
    libManager.loadBooksIntoLibrary();

    AuthenticateManager authManager;

    int choice = -1;
    cout << "\"Welcome to Library Management System!\"" << endl;
    cout << "-----------------------------------------" << endl;
    
    do {
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "0. Exit" << endl;
        cout << "Choose an option (0-2): ";
        cin >> choice;

        switch (choice) {
            case 1:
                execRegister(authManager);
                break;
            case 2:
                execLogin(authManager);
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;

            default:
                cout << "Invalid option. Exiting..." << endl;
        }
    } while (1 <= choice && choice <= 2);
    libManager.saveBooksNewInfo();
}

int main() 
{
    ensureDataDirectoryExists();
    printMenu();
    return 0;
}