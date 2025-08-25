#include "patterns\observer\LibrarianObserver.hpp"

void LibrarianObserver::update(const string &message, const Book *book, Loan *loan)
{
    // Handle the update logic for the librarian
    // Librarians need more detailed information for management purposes

    cout << "=== Librarian System Alert ===" << endl;
    cout << "Alert: " << message << endl;

    if (book)
    {
        // Notify about the book with detailed information
        cout << "Book Management Info:" << endl;
        cout << "  - ISBN: " << book->getISBN() << endl;
        cout << "  - Title: " << book->getTitle() << endl;
        cout << "  - Author: " << book->getAuthor().getName() << endl;
        cout << endl;
        cout << "  - Availability: " << (book->isAvailable() ? "Available" : "Not Available") << endl;
        // cout << "  - Category: " << book->getCategory() << endl;
        // cout << "  - Publication Year: " << book->getPublicationYear() << endl;

        // Additional librarian-specific info
        if (!book->isAvailable())
        {
            cout << "  - ACTION REQUIRED: Book needs to be tracked for return" << endl;
        }
    }

    if (loan)
    {
        // Notify about the loan with management details
        cout << "Loan Management Info:" << endl;
        cout << "  - Loan ID: " << loan->getLoanID() << endl;
        cout << "  - Member ID: " << loan->getMemberID() << endl;
        cout << "  - Book ISBN: " << loan->getBookISBN() << endl;

        // Check loan status and provide management actions
        if (loan->isOverdue())
        {
            cout << "  - URGENT: This loan is OVERDUE!" << endl;
            cout << "  - ACTION REQUIRED: Contact member and apply late fees" << endl;
        }

        // Additional management info
        // cout << "  - Days since loan: " << loan->getDaysSinceLoan() << endl;
    }

    // System timestamp for records
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    cout << "System Timestamp: " << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S") << endl;
    cout << "================================" << endl;

    // Log to librarian system
    logNotification(message, book, loan);
}

void LibrarianObserver::logNotification(const string &message, const Book *book, Loan *loan)
{
    ofstream logFile("librarian_system.log", ios::app);
    if (logFile.is_open())
    {
        // Get current time and convert to string
        auto now = chrono::system_clock::now();
        auto time_t = chrono::system_clock::to_time_t(now);

        logFile << "[LIBRARIAN] [" << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "] ";
        logFile << message;

        if (book)
        {
            logFile << " | Book: " << book->getTitle() << " (ISBN: " << book->getISBN() << ")";
        }

        if (loan)
        {
            logFile << " | Loan ID: " << loan->getLoanID() << " | Member: " << loan->getMemberID();
            if (loan->isOverdue())
            {
                logFile << " | STATUS: OVERDUE";
            }
        }

        logFile << endl;
        logFile.close();
    }
}