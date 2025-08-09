#pragma once

#include "core/User.hpp"
#include "utils/CSVHandler.hpp"

using namespace std;

class AuthenticateManager {
public:
    bool registerUser(const string &userName, const string &password, Role role);
    User *loginUser(const string &userName, const string &password);
    void logoutUser();
};