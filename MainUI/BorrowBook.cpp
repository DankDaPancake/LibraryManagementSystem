#include "imgui.h"
#include "AppState.hpp"                   
#include "services/LibraryManager.hpp"
#include "patterns/strategy/TitleSearchStrategy.hpp"
#include <string>
#include <cstring>
#include <algorithm>

//extern User curUser;

void BorrowBookUI(AppState& appState) {
    static char memberID[64] = "";
    static char isbn[64] = "";
    static char message[256] = "";
 
    auto trim = [](std::string& s){
        auto issp = [](unsigned char c){ return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
    };
 
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        appState = AppState::MainMenu;
        return;
    }
 
    ImGui::Begin("Borrow Book");
    ImGui::InputText("Member ID", memberID, IM_ARRAYSIZE(memberID));
    ImGui::InputText("Book ISBN", isbn, IM_ARRAYSIZE(isbn));
 
    if (ImGui::Button("Borrow###BorrowBookButton")) {
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
            } else {
                Book* b = manager.findBook(code);
                if (!b)       std::snprintf(message, sizeof(message), "ISBN not found.");
                else if (b->getAvailableCopies() <= 0) std::snprintf(message, sizeof(message), "No copies available.");
                else           std::snprintf(message, sizeof(message), "Not available or already borrowed.");
            }
        }
    }
 
    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", message);
 
    if (ImGui::Button("Back to Main Menu")) {
        appState = AppState::MainMenu;
        ImGui::End();
        return;
    }
    ImGui::End();
}