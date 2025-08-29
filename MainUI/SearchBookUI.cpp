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

    if (ImGui::BeginTable("Book Table", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("ISBN", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Author", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Author ID", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Category", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Category ID", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("Available / Total", ImGuiTableColumnFlags_None);
        ImGui::TableHeadersRow(); 

        for (Book* book : results) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", book->getISBN().c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", book->getTitle().c_str());

            ImGui::TableSetColumnIndex(2);
            const Author& a = book->getAuthor();
            ImGui::Text("%s", a.getName().c_str());

            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%d", a.getAuthorID());

            ImGui::TableSetColumnIndex(4);
            const Category& c = book->getCategory();
            ImGui::Text("%s", c.getName().c_str());

            ImGui::TableSetColumnIndex(5);
            ImGui::Text("%d", c.getCategoryID());

            ImGui::TableSetColumnIndex(6);
            ImGui::Text("%d / %d", book->getAvailableCopies(), book->getTotalCopies());
        }

        ImGui::EndTable(); 
    }
}
