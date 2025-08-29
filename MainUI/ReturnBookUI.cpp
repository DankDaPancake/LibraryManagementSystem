#include "imgui.h"
#include "AppState.hpp"                   
#include "services/LibraryManager.hpp"
#include <string>
#include <cstring>
#include <algorithm>

extern User curUser;

void ReturnBookUI(AppState& appState) {
    static string lastUserId;
    static char memberID[64] = "";
    static char isbn[64]     = "";
    static char message[256] = "";

    if (lastUserId != curUser.getUserID()) {
        lastUserId = curUser.getUserID();
        memberID[0] = '\0';
        isbn[0] = '\0';
        message[0] = '\0';
    }
 
    auto trim = [](std::string& s) {
        auto issp = [](unsigned char c){ return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
    };
 
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        appState = AppState::MainMenu;
        return;
    }
 
    ImGui::Text("Search Book");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));
 
    ImGui::InputText("Member ID", memberID, IM_ARRAYSIZE(memberID));
    ImGui::InputText("Book ISBN",  isbn,     IM_ARRAYSIZE(isbn));
 
    if (ImGui::Button("Return###ReturnBookButton")) {
        std::string mid = memberID;
        std::string code = isbn;
        trim(mid); trim(code);
 
        if (mid.empty() || code.empty()) {
            std::snprintf(message, sizeof(message), "Please fill in both Member ID and ISBN.");
        } else {
            auto& manager = LibraryManager::getInstance();
            if (!manager.findMember(mid)) {
                std::snprintf(message, sizeof(message), "Member not found.");
            } else if (!manager.findBook(code)) {
                std::snprintf(message, sizeof(message), "Book not found.");
            } else {
                if (manager.returnBook(mid, code)) {
                    manager.saveBooksNewInfo();  
                    std::snprintf(message, sizeof(message), "Book returned successfully!");
                    isbn[0] = '\0'; 
                } else {
                    std::snprintf(message, sizeof(message), "Return failed. No active loan for this member & ISBN.");
                }
            }
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        memberID[0] = '\0'; 
        isbn[0] = '\0'; 
        message[0] = '\0'; 
    }
}