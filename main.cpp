#include <iostream>
#include "core/Author.hpp"

using namespace std;

int main()
{
    Author aut("10001", "Dang Lee", "I liek cars.");
    cout << aut.getAuthorID() << endl;
    cout << aut.getName() << endl;
    cout << aut.getBiograph() << endl;
}