#include "imgui.h"
#include "AppState.hpp"
#include "services/LibraryManager.hpp"

#include "patterns/strategy/TitleSearchStrategy.hpp"
#include "patterns/strategy/AuthorSearchStrategy.hpp"
#include "patterns/strategy/CategorySearchStrategy.hpp"

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>   
#include <cstdio>  

extern User curUser;

void SearchBookUI(AppState &appState) {
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        appState = AppState::MainMenu;
        return;
    }

    ImGui::Text("Search Book");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));  

    static string lastUserId;
    static int mode = 0;
    const char* modes[] = { "Title (Name)", "Author Name", "Category Name" };
    ImGui::Combo("Mode", &mode, modes, IM_ARRAYSIZE(modes));

    static char queryBuf[128] = "";
    ImGui::InputText("Keyword", queryBuf, IM_ARRAYSIZE(queryBuf));

    static std::vector<Book*> results;
    static char message[256] = "";

    if (lastUserId != curUser.getUserID()) {
        lastUserId = curUser.getUserID();
        mode = 0;
        queryBuf[0] = '\0';
        results.clear();
        message[0] = '\0';
    }

    auto trim = [](std::string& s){
        auto issp = [](unsigned char c){ return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
    };

    if (ImGui::Button("Search")) {
        std::string q = queryBuf;
        trim(q);
        results.clear();
        std::snprintf(message, sizeof(message), "%s", "");

        auto& mgr   = LibraryManager::getInstance();
        const auto& books = mgr.getBooks();   

        switch (mode) {
            case 0: { TitleSearchStrategy    s; results = s.search(books, q); break; }
            case 1: { AuthorSearchStrategy   s; results = s.search(books, q); break; }
            case 2: { CategorySearchStrategy s; results = s.search(books, q); break; }
        }

        if (results.empty()) {
            std::snprintf(message, sizeof(message), "No result found.");
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear")) { 
        mode = 0; queryBuf[0] = '\0'; results.clear(); message[0] = '\0'; 
    }

    if (message[0]) {
        ImGui::Separator();
        ImGui::TextColored(ImVec4(1,1,0,1), "%s", message);
    }

    for (Book* book : results) {
        ImGui::Text("ISBN: %s",  book->getISBN().c_str());
        ImGui::Text("Title: %s", book->getTitle().c_str());

        const Author& a = book->getAuthor();
        ImGui::Text("Author: %s (ID: %d)", a.getName().c_str(), a.getAuthorID());

        const Category& c = book->getCategory();
        ImGui::Text("Category: %s (ID: %d)", c.getName().c_str(), c.getCategoryID());

        ImGui::Text("Status: %s",  book->bookStatusToString().c_str());
        ImGui::Text("Available: %d / %d", book->getAvailableCopies(), book->getTotalCopies());
        ImGui::Separator();
    }
}
