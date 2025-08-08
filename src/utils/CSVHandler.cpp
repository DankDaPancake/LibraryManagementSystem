#include "utils/CSVHandler.hpp"

vector<string> CSVHandler::parseCSVLine(const string &line) {
    vector<string> result;
    stringstream ss(line);
    string item;
    
    while (getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}

bool CSVHandler::userExists(const string &userID, const string &fileName) {
    ifstream file(fileName);
    string line;
    getline(file, line); // skip first line

    while (getline(file, line)) {
        auto items = parseCSVLine(line);
        if (!items.empty() && items[0] == userID) {
            return true;
        }
    }
    
    return false;
}

string CSVHandler::validateCredentials(const string &userName, const string &password, const string &fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error opening file: " << fileName << endl;
        return "Unknown";
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        auto items = parseCSVLine(line);
        if (items.size() >= 3 && items[1] == userName && items[2] == password) {
            return items[0];
        }
    }

    return "Unknown";
}

bool CSVHandler::addUser(const string &userID, const string &userName, const string &password, const string &fileName) {
    if (userExists(userID, fileName)) {
        return false;
    }

    ofstream file(fileName, ios::app);
    if (!file) {
        cout << "Error opening file: " << fileName << endl;
        return false;
    }

    file << userID << ',' << userName << ',' << password << '\n';
    return true;
}

int CSVHandler::memberCount = 0;
int CSVHandler::librarianCount = 0;

string CSVHandler::generateUserID(const string &prefix) {
    stringstream ss;
    int countUser;
    countUser = (prefix == "M"? memberCount: librarianCount);

    ss << prefix << setfill('0') << setw(3) << (++countUser);
    return ss.str();
}
