#pragma once

#include <iostream>

using namespace std;

enum class Role {
    LIBRARIAN,
    MEMBER
};

class User {
private:
    string userID;
    string userName;
    string password;
    Role role;

public:
    User(const string &userID, const string &userName, const string &password, Role role);
    virtual ~User() = default;

    string getUserID() const;
    string getUserName() const;
    Role getRole() const;

    bool login();
    bool register_();
    void viewProfile() const;
};