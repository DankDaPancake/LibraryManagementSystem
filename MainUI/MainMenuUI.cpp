#include "imgui.h"
#include "Screens.hpp"
#include "AppState.hpp"
#include "core/User.hpp"          
#include <vector>
#include <cstring>
#include <string>

extern User curUser;              

void MainMenuUI(AppState &appState) {
    ImGui::Begin("Main Menu");

    const std::string curUsername = curUser.getUserName();
    ImGui::Text("USER: %s", curUsername.c_str());
    ImGui::Text("STATUS: %s", roleToString(curUser.getRole()));
    ImGui::SameLine(250);

    ImGui::BeginGroup();
    ImGui::TextColored(ImVec4(0.1f, 0.7f, 0.3f, 1.0f), "Welcome to Library!");
    ImGui::TextWrapped("If u dont know, this is an Library Management System belongs to HCMUS. "
                       "Hope u'll have a good experience using this service :3");
    ImGui::EndGroup();

    ImGui::Separator();

    static int  selectedItem = 0;
    static Role lastRole     = curUser.getRole();
    if (curUser.getRole() != lastRole) { selectedItem = 0; lastRole = curUser.getRole(); }

    std::vector<const char*> actions = { "Borrow Book", "Return Book", "Search Book", "Find Author", "Find Category" };
    if (curUser.getRole() == Role::LIBRARIAN) actions.push_back("Add Book");

    if (selectedItem >= (int)actions.size()) selectedItem = (int)actions.size() - 1;

    const int count = (int)actions.size();
    std::vector<const char*> items = actions;  // tránh overflow khi thêm mục
    ImGui::Text("Choose Action:"); ImGui::SameLine();
    ImGui::Combo("##ActionCombo", &selectedItem, items.data(), count);

    if (ImGui::Button("Confirm")) {
        const char* chosen = items[selectedItem];
        if      (std::strcmp(chosen, "Borrow Book")   == 0) appState = AppState::BorrowBook;
        else if (std::strcmp(chosen, "Return Book")   == 0) appState = AppState::ReturnBook;
        else if (std::strcmp(chosen, "Search Book")   == 0) appState = AppState::SearchBook;
        else if (std::strcmp(chosen, "Find Author")   == 0) appState = AppState::FindAuthor;
        else if (std::strcmp(chosen, "Find Category") == 0) appState = AppState::FindCategory;
        else if (std::strcmp(chosen, "Add Book")      == 0) appState = AppState::AddBook;
        ImGui::End();
        return;  // kết thúc frame hiện tại
    }

    if (ImGui::Button("Log out")) {
        curUser = User();
        selectedItem = 0;
        appState = AppState::Login;
        ImGui::End();
        return;  
    }

    ImGui::End();
}
