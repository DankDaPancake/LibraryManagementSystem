#include "patterns\decorator\DifficultyLabelDecorator.hpp"

DifficultyLabelDecorator::DifficultyLabelDecorator(shared_ptr<Book> book, const string &label)
        : BookDecorator(book), difficultyLabel(label) {}

string DifficultyLabelDecorator::getFullDescription() const {
    return BookDecorator::getFullDescription() + "," + difficultyLabel;
}