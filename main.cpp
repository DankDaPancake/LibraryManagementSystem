#include <iostream>
#include "core/Author.hpp"
#include "core/Book.hpp"
#include "core/Librarian.hpp"
#include "core/Loan.hpp"
#include "core/User.hpp"
#include "services/LibraryManager.hpp"

using namespace std;

void testAuthor() {
    Author aut("10001", "Dang Lee", "I liek cars.");
    cout << aut.getAuthorID() << endl;
    cout << aut.getName() << endl;
    cout << aut.getBiograph() << endl;
}

void testBook() {
    Book bok("90032", "If Life Actually Gave You Lemons", "12", vector<string>{"10001"}, 5);
    bok.displayBasicInfo();
    cout << bok.getFullDescription() << endl;
    if (bok.isAvailable())
        cout << "Bok is available" << endl;
}

void testUser() {
    User usr("M001", "Phuc", "nightshade", Role::LIBRARIAN);
    usr.viewProfile();
}

void testLibraryManager() {
    LibraryManager &libraryManager = LibraryManager::getInstance();

    // Add a book to the library
    Book *bok = new Book("90032", "If Life Actually Gave You Lemons", "12", vector<string>{"10001"}, 5);
    libraryManager.addBookToSystem(bok);

    // Add a user to the library
    Member *mem = new Member("M001", "Phuc", "nightshade");
    libraryManager.addMemberToSystem(mem);
}

int main() {
    testAuthor();
    testBook();
    testUser();
    testLibraryManager();

    return 0;
}