#pragma once

#include "core/User.hpp"

using namespace std;

class AuthenticateManager {
public:
    bool registerUser(const string &userName, const string &password);
    User *loginUser(const string &userName, const string &password);
    void logoutUser();
};