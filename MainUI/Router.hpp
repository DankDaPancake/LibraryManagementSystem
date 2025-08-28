#include "Screens.hpp"

void RenderUI(AppState& appState) {
    if      (appState == AppState::Login)        LoginUI(appState);
    else if (appState == AppState::Register)     RegisterUI(appState);
    else if (appState == AppState::MainMenu)     MainMenuUI(appState);
    else if (appState == AppState::SearchBook)   SearchBookUI(appState);
    else if (appState == AppState::BorrowBook)   BorrowBookUI(appState);
    else if (appState == AppState::ReturnBook)   ReturnBookUI(appState);
    else if (appState == AppState::FindAuthor)   FindAuthorUI(appState);
    else if (appState == AppState::FindCategory) FindCategoryUI(appState);
    else if (appState == AppState::AddBook)      AddBookUI(appState);
}

