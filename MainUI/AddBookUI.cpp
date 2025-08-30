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

    const float headerPad = 6.0f;
    ImGui::Dummy(ImVec2(0, headerPad));
    ImGui::Text("Book Manage");

    ImGui::SameLine(ImGui::GetWindowWidth() - 300);
    if (ImGui::Button("Add Book")) {
        ImGui::OpenPopup("Add Book Popup");
    }

    if (ImGui::BeginPopupModal("Add Book Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Enter Book Details:");

        ImGui::InputText("ISBN", isbn, IM_ARRAYSIZE(isbn));
        ImGui::InputText("Title", title, IM_ARRAYSIZE(title));
        ImGui::InputText("Author ID", authorID, IM_ARRAYSIZE(authorID));
        ImGui::InputText("Category ID", categoryID, IM_ARRAYSIZE(categoryID));
        ImGui::InputInt("Total Copies", &totalCopies);
        if (totalCopies < 1) totalCopies = 1;

        if (ImGui::Button("Add")) {
            auto trim = [](std::string& s) {
                auto issp = [](unsigned char c){ return std::isspace(c); };
                s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); })); // Remove leading spaces
                s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end()); // Remove trailing spaces
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
                    ImGui::CloseCurrentPopup();
                    return; 
                }

                auto& manager = LibraryManager::getInstance();

                if (manager.addBook(sISBN, sTitle, aID, cID, totalCopies)) {     
                    manager.saveBooksNewInfo();                                   
                    std::snprintf(message, sizeof(message), "Book added!");
                    isbn[0] = title[0] = authorID[0] = categoryID[0] = '\0'; totalCopies = 1;
                    ImGui::CloseCurrentPopup();  
                } else {
                    std::snprintf(message, sizeof(message), "Failed. Check ISBN duplicate or invalid IDs.");
                }
            }
        }

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
        if (ImGui::Button("Close")) { 
            ImGui::CloseCurrentPopup();  
        }

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(1,1,0,1), "%s", message);
        ImGui::EndPopup();
    }

    ImGui::Separator();
    ImGui::Text("Book collection:");

    if (ImGui::BeginTable("Books Table", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ISBN");
        ImGui::TableSetupColumn("Title");
        ImGui::TableSetupColumn("Author");
        ImGui::TableSetupColumn("Category");
        ImGui::TableSetupColumn("Total Copies");
        ImGui::TableHeadersRow();

        auto& manager = LibraryManager::getInstance();
        auto books = manager.getBooks();  

        for (const Book* book : books) { 
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text(book->getISBN().c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text(book->getTitle().c_str());

            const std::string authorName = book->getAuthor().getName();  
            const std::string categoryName = book->getCategory().getName(); 

            ImGui::TableSetColumnIndex(2); ImGui::Text(authorName.c_str());
            ImGui::TableSetColumnIndex(3); ImGui::Text(categoryName.c_str());
            ImGui::TableSetColumnIndex(4); ImGui::Text(std::to_string(book->getTotalCopies()).c_str());
        }

        ImGui::EndTable();
    }
}






