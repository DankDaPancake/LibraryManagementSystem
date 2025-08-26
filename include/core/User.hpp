#pragma once

#include <iostream>

using namespace std;

enum class Role {
    LIBRARIAN = 0,
    MEMBER = 1
};

class User {
private:
    string userID;
    string userName;
    string password;
    Role role;

public:
    User();
    User(const string &name, Role r);
    User(const string &userID, const string &userName, const string &password, Role role);
    virtual ~User() = default;

    string getUserID() const;
    string getUserName() const;
    Role getRole() const;

    void viewProfile() const;
};