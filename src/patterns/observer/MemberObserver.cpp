#include "patterns\observer\MemberObserver.hpp"

void MemberObserver::update(const string &message, const Book *book, Loan *loan)
{
    // Handle the update logic for the member
    // For example, notify the member about a new book or loan status

    cout << "=== Member Notification ===" << endl;
    cout << "Message: " << message << endl;

    if (book)
    {
        // Notify about the book
        cout << "Book Information:" << endl;
        cout << "  - ISBN: " << book->getISBN() << endl;
        cout << "  - Title: " << book->getTitle() << endl;
        cout << "  - Author: " << book->getAuthor().getName() << endl;
        cout << endl;
        cout << "  - Status: " << (book->isAvailable() ? "Available" : "Not Available") << endl;
    }

    if (loan)
    {
        // Notify about the loan
        cout << "Loan Information:" << endl;
        cout << "  - Loan ID: " << loan->getLoanID() << endl;
        cout << "  - Due Date: " << Loan::dateToString(loan->getDueDate()) << endl;
        cout << "  - Status: " << Loan::loanStatusToString(loan->getStatus()) << endl;

        // Check if loan is overdue and send warning
        if (loan->isOverdue())
        {
            cout << "  - WARNING: This loan is overdue!" << endl;
        }
    }

    // Log the notification
    auto now = chrono::system_clock::now();
    auto time_t = chrono::system_clock::to_time_t(now);
    cout << "Timestamp: " << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S") << endl;
    cout << "=========================" << endl;

    logNotification(message, book, loan);
}

void MemberObserver::logNotification(const string &message, const Book *book, Loan *loan)
{
    ofstream logFile("../librarylog/member_notifications.log", ios::app);
    if (logFile.is_open())
    {
        // Get current time and convert to string
        auto now = chrono::system_clock::now();
        auto time_t = chrono::system_clock::to_time_t(now);

        logFile << "[" << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "] ";
        logFile << message;
        if (book)
        {
            logFile << " | Book: " << book->getTitle();
        }
        if (loan)
        {
            logFile << " | Loan ID: " << loan->getLoanID();
        }
        logFile << endl;
        logFile.close();
    }
}