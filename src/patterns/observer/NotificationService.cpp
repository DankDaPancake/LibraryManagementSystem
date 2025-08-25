#include "patterns\observer\NotificationService.hpp"

void NotificationService::sendDueSoonNotifications()
{
    auto now = system_clock::now();
    auto &loans = libraryManager->loans;

    for (auto loan : loans)
    {
        auto hoursLeft = duration_cast<hours>(loan->getDueDate() - now).count();
        if (loan->getStatus() == LoanStatus::ACTIVE && hoursLeft <= 48)
        {
            cout << "- Loan ID: " << loan->getLoanID() << " | Due in " << hoursLeft << " hours." << endl;
        }
    }
}

void NotificationService::checkAndNotifyDueLoans()
{
    auto &loans = libraryManager->loans;

    for (auto loan : loans)
    {
        if (loan->isOverdue())
        {
            cout << "- Loan ID: " << loan->getLoanID() << " is overdue." << endl;
            ;
        }
    }
}

void NotificationService::sendReservationAvailableNotifications()
{
    auto &books = libraryManager->books;

    for (auto book : books)
    {
        if (book->isAvailable())
        {
            cout << "- Book: " << book->getFullDescription() << " is now available." << endl;
        }
    }
}
