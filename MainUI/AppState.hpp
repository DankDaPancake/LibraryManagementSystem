#pragma once
#include "core/User.hpp"

enum class AppState {
    Login,
    MainMenu,
    Register,
    SearchBook,
    BorrowBook,
    ReturnBook,
    AddBook,
    ListMember,
    ListLoan,
    MemberManage
};

inline const char* roleToString(Role role) {
    switch (role) {
        case Role::LIBRARIAN: return "LIBRARIAN";
        case Role::MEMBER:    return "MEMBER";
        default:              return "UNKNOWN";
    }
}
