#include "imgui.h"
#include "AppState.hpp"                   
#include "services/LibraryManager.hpp"
#include "core/User.hpp"

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>   
#include <cstdio> 

extern User curUser;

// void BorrowBookUI(AppState& appState) {
//     static std::string lastUserId;
//     static char memberID[64] = "";
//     static char isbn[64]     = "";
//     static char message[256] = "";

//     if (lastUserId != curUser.getUserID()) {
//         lastUserId = curUser.getUserID();
//         memberID[0] = isbn[0] = message[0] = '\0';
//     }
//     auto trim = [](std::string& s){ 
//         auto issp = [](unsigned char c){ return std::isspace(c); };
//         s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
//         s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
//     };

//     ImGui::Text("Borrow Book");
//     ImGui::Separator();
//     ImGui::Dummy(ImVec2(0, 6));

//     bool submit = false;

//     if (ImGui::BeginTable("borrow_form", 2, ImGuiTableFlags_SizingStretchProp)) {
//         ImGui::TableNextRow();
//         ImGui::TableSetColumnIndex(0); ImGui::TextUnformatted("Member ID");
//         ImGui::TableSetColumnIndex(1); ImGui::SetNextItemWidth(-FLT_MIN);
//         submit |= ImGui::InputText("##member", memberID, IM_ARRAYSIZE(memberID),
//                                    ImGuiInputTextFlags_EnterReturnsTrue);

//         ImGui::TableNextRow();
//         ImGui::TableSetColumnIndex(0); ImGui::TextUnformatted("Book ISBN");
//         ImGui::TableSetColumnIndex(1); ImGui::SetNextItemWidth(-FLT_MIN);
//         submit |= ImGui::InputText("##isbn", isbn, IM_ARRAYSIZE(isbn),
//                                    ImGuiInputTextFlags_EnterReturnsTrue);
//         ImGui::EndTable();
//     }

//     ImGui::Dummy(ImVec2(0, 6));

//     if (ImGui::Button("Borrow", ImVec2(140, 0)) || submit) {
//         std::string mid = memberID, code = isbn; trim(mid); trim(code);
//         if (mid.empty() || code.empty()) {
//             std::snprintf(message, sizeof(message), "Please fill in both Member ID and ISBN.");
//         } else {
//             auto& manager = LibraryManager::getInstance();
//             if (manager.borrowBook(mid, code)) {
//                 manager.saveBooksNewInfo();
//                 std::snprintf(message, sizeof(message), "Book borrowed successfully!");
//                 isbn[0] = '\0';
//             } else {
//                 Book* b = manager.findBook(code);
//                 if (!b)                               std::snprintf(message, sizeof(message), "ISBN not found.");
//                 else if (b->getAvailableCopies() <= 0) std::snprintf(message, sizeof(message), "No copies available.");
//                 else                                   std::snprintf(message, sizeof(message), "Not available or already borrowed.");
//             }
//         }
//     }
//     ImGui::SameLine();
//     if (ImGui::Button("Clear")) { memberID[0] = '\0'; isbn[0] = '\0'; }

//     if (message[0]) { ImGui::Dummy(ImVec2(0, 8)); ImGui::TextColored(ImVec4(1,1,0,1), "%s", message); }
// }

void BorrowBookUI(AppState& appState) {
    static std::string lastUserId;
    static char memberID[64] = "";
    static char isbn[64] = "";
    static int dueDate = 30; // Ngày mượn
    static char message[256] = "";

    if (lastUserId != curUser.getUserID()) {
        lastUserId = curUser.getUserID();
        memberID[0] = isbn[0] = message[0] = '\0';
    }

    auto trim = [](std::string& s) { 
        auto issp = [](unsigned char c){ return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
    };

    ImGui::Text("List of Books Currently Borrowed:");

    if (ImGui::BeginTable("loan_table", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Loan ID");
        ImGui::TableSetupColumn("Book ISBN");
        ImGui::TableSetupColumn("Book Title");
        ImGui::TableSetupColumn("Member ID");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Return Date");
        ImGui::TableHeadersRow();

        auto& manager = LibraryManager::getInstance();
        auto loans = manager.getLoans();  
        for (const auto& loan : loans) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text(loan->getLoanID().c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text(loan->getBookISBN().c_str());
            //ImGui::TableSetColumnIndex(2); ImGui::Text(loan->getBookTitle(manager).c_str());
            ImGui::TableSetColumnIndex(3); ImGui::Text(loan->getMemberID().c_str());
            // ImGui::TableSetColumnIndex(4); ImGui::Text(loan.getStatus().c_str());
            // ImGui::TableSetColumnIndex(5); ImGui::Text(loan.getReturnDate().c_str());
        }

        ImGui::EndTable();
    }

    if (ImGui::Button("Borrow Book")) {
        ImGui::OpenPopup("Borrow Book Popup");
    }

    if (ImGui::BeginPopupModal("Borrow Book Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Enter Borrow Book Details:");

        ImGui::InputText("Member ID", memberID, IM_ARRAYSIZE(memberID));
        ImGui::InputText("Book ISBN", isbn, IM_ARRAYSIZE(isbn));
        ImGui::InputInt("Borrow Date", &dueDate);

        if (ImGui::Button("Borrow")) {
            std::string mid = memberID, code = isbn;
            trim(mid); trim(code);

            if (mid.empty() || code.empty()) {
                std::snprintf(message, sizeof(message), "Please fill in both Member ID and ISBN.");
            } else {
                auto& manager = LibraryManager::getInstance();
                if (manager.borrowBook(mid, code)) {
                    manager.saveBooksNewInfo();
                    std::snprintf(message, sizeof(message), "Book borrowed successfully!");
                    isbn[0] = '\0';
                    ImGui::CloseCurrentPopup();
                } else {
                    Book* b = manager.findBook(code);
                    if (!b) std::snprintf(message, sizeof(message), "ISBN not found.");
                    else if (b->getAvailableCopies() <= 0) std::snprintf(message, sizeof(message), "No copies available.");
                    else std::snprintf(message, sizeof(message), "Not available or already borrowed.");
                }
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }

        if (message[0]) {
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", message);
        }

        ImGui::EndPopup();
    }
}





