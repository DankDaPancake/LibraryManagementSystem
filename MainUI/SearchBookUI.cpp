#include "imgui.h"
#include "AppState.hpp"                   
#include "services/LibraryManager.hpp"
#include "patterns/strategy/TitleSearchStrategy.hpp"
#include <string>
#include <cstring>
#include <vector>

//extern User curUser;

void SearchBookUI(AppState &appState) {
    static char query[128] = "";
    static std::vector<Book*> results;

    LibraryManager& manager = LibraryManager::getInstance();
    static TitleSearchStrategy titleStrategy;
    manager.setSearchStrategy(&titleStrategy);

    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        appState = AppState::MainMenu;
        return;
    }

    ImGui::Begin("Search Book");

    ImGui::InputText("Enter title", query, IM_ARRAYSIZE(query));
    if (ImGui::Button("Search")) {
        results = manager.searchBooks(query); 
    }

    ImGui::Separator();
    ImGui::Text("Result List");

    if (!results.empty()) {
        for (Book* book : results) {
            if (!book) continue;
            ImGui::Separator();

            ImGui::Text("ISBN: %s", book->getISBN().c_str());
            ImGui::Text("Title: %s", book->getTitle().c_str());

            const Author& a = book->getAuthor();
            ImGui::Text("Author: %s (ID: %s)", a.getName().c_str(), a.getAuthorID().c_str());

            const Category& c = book->getCategory();
            ImGui::Text("Category: %s (ID: %s)", c.getName().c_str(), c.getCategoryID().c_str());

            ImGui::Text("Status: %s", book->bookStatusToString().c_str());
            ImGui::Text("Available: %d / %d",
                        book->getAvailableCopies(),
                        book->getTotalCopies());
        }
    } else {
        ImGui::Text("No matching book found.");
    }

    ImGui::Spacing();

    if (ImGui::Button("Back to Main Menu")) {
        appState = AppState::MainMenu;
        ImGui::End();
        return;
    }


    ImGui::End();
}