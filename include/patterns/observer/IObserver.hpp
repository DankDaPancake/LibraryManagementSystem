#pragma once

#include "core/Book.hpp"
#include "core/Loan.hpp"

#include <chrono>
#include <string>
#include <iomanip>
#include <ctime>

using namespace std;

class IObserver {
public:
    IObserver() = default;
    virtual ~IObserver() = default;
    
    virtual void update(const string &message, const Book *book,  Loan *loan) = 0;
    virtual void logNotification(const string &message, const Book *book, Loan *loan) = 0;
};