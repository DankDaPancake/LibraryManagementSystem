#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class CSVHandler {
public:
    static vector<string> parseCSVLine(const string &line);
    static bool userExists(const string &userID, const string &fileName);
    static string validateCredentials(const string &userName, const string &password, const string &fileName);
    static bool addUser(const string &userID, const string &userName, const string &password, const string &fileName);
    static string generateUserID(const string &prefix);
};