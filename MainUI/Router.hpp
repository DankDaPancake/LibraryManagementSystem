#include "Screens.hpp"

void RenderUI(AppState& appState) {
    if      (appState == AppState::Login)        LoginUI(appState);
    else if (appState == AppState::Register)     RegisterUI(appState);
    else if (appState == AppState::MainMenu)     MainMenuUI(appState);
    else if (appState == AppState::SearchBook)   SearchBookUI(appState);
    else if (appState == AppState::BorrowBook)   BorrowBookUI(appState);
    else if (appState == AppState::ListMember)   ListMemberUI(appState);
    else if (appState == AppState::ListLoan)   ListLoanUI(appState);
    else if (appState == AppState::MemberManage)   MemberManageUI(appState);
    else if (appState == AppState::AddBook)      AddBookUI(appState);
    else if (appState == AppState::Notification)      NotificationUI(appState);
}

