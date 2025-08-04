#include <iostream>
#include "core/Author.hpp"
#include "core/Book.hpp"
#include "core/User.hpp"
#include "core/Loan.hpp"

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

void testLoan() {
}

int main()
{
    testAuthor();
    testBook();
    testUser();
}