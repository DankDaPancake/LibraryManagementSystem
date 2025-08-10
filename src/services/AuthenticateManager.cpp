#include "services/AuthenticateManager.hpp"

bool AuthenticateManager::registerUser(const string &userName, const string &password, Role role) {
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
    if (userID[0] == 'L') {
        return new User(userID, userName, password, Role::LIBRARIAN);
    }

    userID = CSVHandler::validateCredentials(userName, password, "../data/members.csv");
    if (userID[0] == 'M') {
        return new User(userID, userName, password, Role::MEMBER);
    }

    cout << "Invalid credentials. Please try again." << endl;
    return nullptr;
}

void AuthenticateManager::logoutUser() {
    cout << "Successfully logged out." << endl;
}