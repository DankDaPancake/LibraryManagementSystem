#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>

#include "core/User.hpp"
#include "core/Book.hpp"
#include "core/Loan.hpp"
#include "core/Member.hpp"
#include "services/LibraryManager.hpp"

using namespace std;

class Librarian: public User {
public:
    Librarian(const string &userID, const string &userName, const string &password);

    void addBook(Book *book);
    void removeBook(const string &ISBN);
    void updateBookInfo(Book *book);
    void manageMemberInfo(Member *member);

    vector<Loan *> viewAllLoans();
};