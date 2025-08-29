#include "imgui.h"
#include "AppState.hpp"                   
#include "services/LibraryManager.hpp"
#include "core/User.hpp"
#include <algorithm>
#include <string>
#include <cstring>

extern User curUser;

void AddBookUI(AppState& appState) {
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { 
        appState = AppState::MainMenu; 
        return; 
    }

    if (curUser.getRole() != Role::LIBRARIAN) { 
        appState = AppState::MainMenu; 
        return; 
    }

    static std::string lastUserId;
    static char isbn[32] = "";
    static char title[128] = "";
    static char authorID[16] = "";
    static char categoryID[16] = "";
    static int totalCopies = 1;
    static char message[256] = "";

    if (lastUserId != curUser.getUserID()) {
        lastUserId = curUser.getUserID();
        isbn[0] = '\0';
        message[0] = '\0';
        categoryID[0] = '\0';
        title[0] = '\0';
        authorID[0] = '\0';
        totalCopies = 1;
    }

    ImGui::Separator();

    ImGui::InputText("ISBN", isbn, IM_ARRAYSIZE(isbn));
    ImGui::InputText("Title", title, IM_ARRAYSIZE(title));
    ImGui::InputText("Author ID", authorID, IM_ARRAYSIZE(authorID));
    ImGui::InputText("Category ID", categoryID, IM_ARRAYSIZE(categoryID));
    ImGui::InputInt("Total Copies", &totalCopies);
    if (totalCopies < 1) totalCopies = 1;

    if (ImGui::Button("Add Book")) {
        auto trim = [](std::string& s) {
            auto issp = [](unsigned char c){ return std::isspace(c); };
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
            s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
        };

        std::string sISBN = isbn, sTitle = title, sAuth = authorID, sCat = categoryID;
        trim(sISBN); trim(sTitle); trim(sAuth); trim(sCat);

        if (sISBN.empty() || sTitle.empty() || sAuth.empty() || sCat.empty()) {
            std::snprintf(message, sizeof(message), "Please fill all fields.");
        } else {
            int aID=0, cID=0;
            try { 
                aID = std::stoi(sAuth); 
                cID = std::stoi(sCat); 
            }
            catch (...) { 
                std::snprintf(message, sizeof(message), "Author/Category ID must be numbers."); 
                goto after; 
            }

            auto& manager = LibraryManager::getInstance();

            if (manager.addBook(sISBN, sTitle, aID, cID, totalCopies)) {     
                manager.saveBooksNewInfo();                                   
                std::snprintf(message, sizeof(message), "Book added!");
                isbn[0]=title[0]=authorID[0]=categoryID[0]='\0'; totalCopies=1;
            } else {
                std::snprintf(message, sizeof(message), "Failed. Check ISBN duplicate or invalid IDs.");
            }
        }
    }
after:
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        isbn[0] = '\0'; 
        title[0] = '\0'; 
        authorID[0] = '\0'; 
        categoryID[0] = '\0'; 
        totalCopies = 1;
        message[0] = '\0';
    }

    ImGui::SameLine();
    if (ImGui::Button("Back to Main Menu")) { 
        isbn[0] = '\0';
        message[0] = '\0';
        categoryID[0] = '\0';
        title[0] = '\0';
        authorID[0] = '\0';
        totalCopies = 1;
        appState = AppState::MainMenu; 
        return; 
    }

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1,1,0,1), "%s", message);
}
