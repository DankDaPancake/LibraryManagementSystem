#include "core/User.hpp"

User::User(const string &userID, const string &userName, const string &password, Role &role) 
    : userID(userID), userName(userName), password(password), role(role) {}

string User::getUserID() const { return userID; }
string User::getUserName() const { return userName; }
Role User::getRole() const { return role; }

bool User::login() {
    // login user with authenticate manager
}

bool User::register_() {
    // register user with authenticate manager
}

void User::viewProfile() const {
    cout << "[User profile]:" << endl;
    cout << "+ ID: " << userID << endl;
    cout << "+ Name: " << userName << endl;
    cout << "+ Role: " << (role == Role::LIBRARIAN ? "Librarian" : "Member") << endl;
}