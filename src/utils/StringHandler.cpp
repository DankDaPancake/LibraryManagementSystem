#include "utils/StringHandler.hpp"
#include <algorithm>
#include <cctype>
#include <limits>

string StringHandler::getLowercase(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return tolower(c); });
    return s;
}

// Levenshtein distance function
int StringHandler::levenshteinDistance(const string &a, const string &b) {
    int m = a.size(), n = b.size();
    vector<int> prev(n + 1), curr(n + 1);

    for (int j = 0; j <= n; j++) prev[j] = j;
    for (int i = 1; i <= m; i++) {
        curr[0] = i;
        for (int j = 1; j <= n; ++j) {
            int cost = (a[i - 1] == b[j - 1]? 0: 1);
            curr[j] = min({prev[j] + 1, curr[j - 1] + 1, prev[j - 1] + cost});
        }
        swap(prev, curr);
    }
    return prev[n];
}

double StringHandler::matchScore(const string &pattern, const string &query) {
    string pLower = getLowercase(pattern);
    string qLower = getLowercase(query);

    if (pLower == qLower) return 100.0;

    if (pLower.find(qLower) != string::npos) return 80.0;

    int dist = levenshteinDistance(pLower, qLower);
    double maxLen = max(pLower.size(), qLower.size());
    double sim = (1.0 - (double)dist / maxLen) * 60.0;
    return sim;
}

// Find top N matches
vector<pair<double, Book *>> StringHandler::findTopMatches(
    const vector<pair<string, Book *>> &patterns, const string &query, int topN) {
    
    vector<pair<double, Book *>> scored;

    for (const auto &[pattern, item] : patterns) {
        double score = matchScore(pattern, query);
        scored.emplace_back(score, item);
    }

    sort(scored.begin(), scored.end(),
        [](const auto &a, const auto &b) { return a.first > b.first;});

    if (scored.size() > topN) scored.resize(topN);

    return scored;
}
