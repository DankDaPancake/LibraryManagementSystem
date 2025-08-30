#include "imgui.h"
#include "AppState.hpp"
#include "services/LibraryManager.hpp"
#include "core/User.hpp"
#include <algorithm>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdio>

extern User curUser;

// State cho popup Add
static std::string s_lastUserId;
static char s_isbn[32]      = "";
static char s_title[128]    = "";
static char s_authorID[16]  = "";
static char s_categoryID[16]= "";
static int  s_totalCopies   = 1;
static char s_msg[256]      = "";

// State cho popup Remove
static char s_rmIsbn[32]    = "";
static char s_rmMsg[256]    = "";

void AddBookUI(AppState& appState)
{
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { appState = AppState::MainMenu; return; }
    if (curUser.getRole() != Role::LIBRARIAN)  { appState = AppState::MainMenu; return; }

    // Dùng lại giữa các popup
    auto& manager = LibraryManager::getInstance();

    // Reset form khi đổi user
    if (s_lastUserId != curUser.getUserID()) {
        s_lastUserId = curUser.getUserID();
        s_isbn[0] = s_title[0] = s_authorID[0] = s_categoryID[0] = 0;
        s_totalCopies = 1;
        s_msg[0] = 0;
        s_rmIsbn[0] = s_rmMsg[0] = 0;
    }

    // Header + nút
    ImGui::Dummy(ImVec2(0, 6));
    ImGui::TextUnformatted("Book Manage");

    float right = ImGui::GetContentRegionAvail().x;
    float btnW = 110.0f;
    ImGui::SameLine(ImGui::GetCursorPosX() + right - btnW*2 - 8.0f);
    if (ImGui::Button("Add", ImVec2(btnW, 0)))
        ImGui::OpenPopup("Add Book Popup");
    ImGui::SameLine();
    if (ImGui::Button("Remove", ImVec2(btnW, 0)))
        ImGui::OpenPopup("Remove Book Popup");

    /* ------------------- Add Book ------------------- */
    if (ImGui::BeginPopupModal("Add Book Popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::TextUnformatted("Enter Book Details:");
        ImGui::InputText("ISBN",        s_isbn,       IM_ARRAYSIZE(s_isbn));
        ImGui::InputText("Title",       s_title,      IM_ARRAYSIZE(s_title));
        ImGui::InputText("Author ID",   s_authorID,   IM_ARRAYSIZE(s_authorID));
        ImGui::InputText("Category ID", s_categoryID, IM_ARRAYSIZE(s_categoryID));
        ImGui::InputInt ("Total Copies", &s_totalCopies);
        if (s_totalCopies < 1) s_totalCopies = 1;

        if (ImGui::Button("Add"))
        {
            auto trim = [](std::string& s){
                auto issp = [](unsigned char c){ return std::isspace(c); };
                s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
                s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
            };

            std::string isbn = s_isbn, title = s_title, a = s_authorID, c = s_categoryID;
            trim(isbn); trim(title); trim(a); trim(c);

            if (isbn.empty() || title.empty() || a.empty() || c.empty()) {
                std::snprintf(s_msg, sizeof(s_msg), "Please fill all fields.");
            } else {
                int aID = 0, cID = 0;
                try { aID = std::stoi(a); cID = std::stoi(c); }
                catch (...) {
                    std::snprintf(s_msg, sizeof(s_msg), "Author/Category ID must be numbers.");
                    // KHÔNG return ở giữa Popup!
                }

                if (s_msg[0] == 0) {
                    if (manager.addBook(isbn, title, aID, cID, s_totalCopies)) {
                        manager.saveBooksNewInfo();
                        std::snprintf(s_msg, sizeof(s_msg), "Book added!");
                        s_isbn[0] = s_title[0] = s_authorID[0] = s_categoryID[0] = 0; s_totalCopies = 1;
                        ImGui::CloseCurrentPopup();
                    } else {
                        std::snprintf(s_msg, sizeof(s_msg), "Failed. Check ISBN duplicate or invalid IDs.");
                    }
                }
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Clear")) {
            s_isbn[0] = s_title[0] = s_authorID[0] = s_categoryID[0] = 0;
            s_totalCopies = 1; s_msg[0] = 0;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close")) ImGui::CloseCurrentPopup();

        if (s_msg[0]) { ImGui::Spacing(); ImGui::TextColored(ImVec4(1,1,0,1), "%s", s_msg); }
        ImGui::EndPopup();
    }

    /* ------------------ Remove Book ----------------- */
    if (ImGui::BeginPopupModal("Remove Book Popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::TextUnformatted("Enter ISBN to remove:");
        ImGui::SetNextItemWidth(320.0f);
        ImGui::InputText("##rm_isbn", s_rmIsbn, IM_ARRAYSIZE(s_rmIsbn));

        const Book* preview = nullptr;
        if (s_rmIsbn[0] != '\0') {
            for (const Book* b : manager.getBooks()) {
                if (b && b->getISBN() == s_rmIsbn) { preview = b; break; }
            }
        }
        if (preview) { ImGui::Spacing(); ImGui::TextDisabled("Found: %s", preview->getTitle().c_str()); }

        ImGui::Spacing();
        if (ImGui::Button("Remove"))
        {
            s_rmMsg[0] = 0;

            Book* target = nullptr;
            // lấy con trỏ ghi (manager.getBooks() trả const ref -> cast về non-const để erase)
            auto& nonConst = const_cast<std::vector<Book*>&>(manager.getBooks());
            for (Book* b : nonConst) { if (b && b->getISBN() == s_rmIsbn) { target = b; break; } }

            if (!target) {
                std::snprintf(s_rmMsg, sizeof(s_rmMsg), "ISBN not found.");
            } else {
                auto it = std::find(nonConst.begin(), nonConst.end(), target);
                if (it != nonConst.end()) {
                    delete *it;
                    nonConst.erase(it);
                    manager.saveBooksNewInfo();
                    std::snprintf(s_rmMsg, sizeof(s_rmMsg), "Removed book with ISBN %s.", s_rmIsbn);
                    s_rmIsbn[0] = 0;
                    ImGui::CloseCurrentPopup();
                } else {
                    std::snprintf(s_rmMsg, sizeof(s_rmMsg), "Unexpected error: iterator not found.");
                }
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel##rm")) ImGui::CloseCurrentPopup();

        if (s_rmMsg[0]) { ImGui::Spacing(); ImGui::TextWrapped("%s", s_rmMsg); }
        ImGui::EndPopup();
    }

    /* ------------------ Bảng liệt kê ----------------- */
    ImGui::Separator();
    ImGui::TextUnformatted("Book collection:");

    if (ImGui::BeginTable("Books Table", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableSetupColumn("ISBN");
        ImGui::TableSetupColumn("Title");
        ImGui::TableSetupColumn("Author");
        ImGui::TableSetupColumn("Category");
        ImGui::TableSetupColumn("Total Copies", ImGuiTableColumnFlags_WidthFixed, 120.0f);
        ImGui::TableHeadersRow();

        for (const Book* book : manager.getBooks()) {
            if (!book) continue;
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::TextUnformatted(book->getISBN().c_str());
            ImGui::TableSetColumnIndex(1); ImGui::TextUnformatted(book->getTitle().c_str());
            ImGui::TableSetColumnIndex(2); ImGui::TextUnformatted(book->getAuthor().getName().c_str());
            ImGui::TableSetColumnIndex(3); ImGui::TextUnformatted(book->getCategory().getName().c_str());
            ImGui::TableSetColumnIndex(4); ImGui::Text("%d", book->getTotalCopies());
        }
        ImGui::EndTable();
    }
}
