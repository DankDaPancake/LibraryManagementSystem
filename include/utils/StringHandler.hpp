#pragma once

#include <string>
#include <vector>
#include "core/Book.hpp"

using namespace std;

class StringHandler {
private:
    static int levenshteinDistance(const string &a, const string &b);
    static double matchScore(const string &author, const string &query);

public:
    static string getLowercase(string s);
    static vector<pair<double, Book *>> findTopMatches(
            const vector<pair<string, Book *>> &patterns, const string &query, int topN);
};