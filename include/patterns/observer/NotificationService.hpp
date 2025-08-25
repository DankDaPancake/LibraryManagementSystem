#pragma once

#include "services/LibraryManager.hpp"

#include <iostream>
#include <chrono>
#include <vector>

using namespace std;
using namespace chrono;

class Loan;
class Book;
class LibraryManager;

class NotificationService {
private:
    LibraryManager *libraryManager;

public:
    void sendDueSoonNotifications();
    void checkAndNotifyDueLoans();
    void sendReservationAvailableNotifications();
};
