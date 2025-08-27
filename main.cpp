#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>

#include <algorithm>
#include <cctype> 

#include "core/User.hpp"
#include "core/Book.hpp"

#include "services/AuthenticateManager.hpp"
#include "services/LibraryManager.hpp"

#include "utils/CSVHandler.hpp"

#include "patterns/strategy/TitleSearchStrategy.hpp"

// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static bool                     g_DeviceLost = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void myStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 0;

    style.Colors[ImGuiCol_Text]                  = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
}

enum class AppState {
    Login,
    MainMenu,
    Register,
    SearchBook,
    BorrowBook,
    ReturnBook,
    AddBook,
    FindAuthor,
    FindCategory
};

const char* roleToString(Role role) {
    switch (role) {
        case Role::LIBRARIAN: return "LIBRARIAN";
        case Role::MEMBER: return "MEMBER";
        default: return "UNKNOWN";
    }
}

AppState appState = AppState::Login;
User curUser;

void RegisterUI(AppState &appState) {
    static string newUsername = "";
    static string newPassword = "";

    static bool registerSuccess = false;
    static bool registerFailed = false;

    static char usernameBuf[64];
    strncpy(usernameBuf, newUsername.c_str(), sizeof(usernameBuf));
    usernameBuf[sizeof(usernameBuf) - 1] = '\0';

    static char passwordBuf[64];
    strncpy(passwordBuf, newPassword.c_str(), sizeof(passwordBuf));
    passwordBuf[sizeof(passwordBuf) - 1] = '\0';

    ImGui::Begin("Register");

    ImGui::Text("Register form here...");
    if (ImGui::InputText("New Username", usernameBuf, IM_ARRAYSIZE(usernameBuf))) {
        newUsername = usernameBuf;
    }
    if (ImGui::InputText("New Password", passwordBuf, IM_ARRAYSIZE(passwordBuf), ImGuiInputTextFlags_Password)) {
        newPassword = passwordBuf;
    }

    struct RoleItem { const char* name; Role value; };
    static RoleItem roleItems[] = {
        {"MEMBER",    Role::MEMBER},
        {"LIBRARIAN", Role::LIBRARIAN},
    };
    static int currentRoleIndex = 0;
    const char* roleNames[] = { roleItems[0].name, roleItems[1].name };
    int roleCount = 2; 

    ImGui::Combo("Role", &currentRoleIndex, roleNames, roleCount);
    Role chosenRole = roleItems[currentRoleIndex].value; 

    if (ImGui::Button("Register")) {
        AuthenticateManager auth;

        if (auth.registerUser(newUsername, newPassword, chosenRole)) {
            registerSuccess = true;
            registerFailed = false;
            appState = AppState::Login;
        } else {
            registerSuccess = false;
            registerFailed = true;
        }
    }

    if (registerSuccess) {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Registered successfully!");
    } else if (registerFailed) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Register failed! Try again.");
    }

    if (ImGui::Button("Back to Login")) {
        appState = AppState::Login;
    }

    ImGui::End();
}


void LoginUI(AppState &appState) {
    static char usernameInput[64] = "";
    static char passwordInput[64] = "";
    Role roleInput;
    bool loginSuccess = false;
    bool loginFailed = false;

    ImGui::Begin("Electronic Library");

    ImGui::InputText("Username", usernameInput, IM_ARRAYSIZE(usernameInput));
    ImGui::InputText("Password", passwordInput, IM_ARRAYSIZE(passwordInput), ImGuiInputTextFlags_Password);

    if (ImGui::Button("Login")) {
        AuthenticateManager auth;
        User* user = auth.loginUser(usernameInput, passwordInput);

        if (user != nullptr) {
            roleInput = user->getRole();
            loginSuccess = true;
            loginFailed = false;
            curUser = *user;              
            delete user;
            appState = AppState::MainMenu;
        } else {
            loginSuccess = false;
            loginFailed = true;
        }
    }

    if (loginSuccess) {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Login Successful!");
    } else if (loginFailed){
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Login Failed!");
    }

    ImGui::Text("Don't have an account?");
    ImGui::SameLine();  
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.6f, 1.0f, 1.0f)); 
    ImGui::Text("Register");
    ImGui::PopStyleColor();

    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsItemClicked()) {
            appState = AppState::Register; 
        }
    }

    ImGui::End();
}

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

void ReturnBookUI(AppState& appState) {
    static char memberID[64] = "";
    static char isbn[64]     = "";
    static char message[256] = "";

    auto trim = [](std::string& s) {
        auto issp = [](unsigned char c){ return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
    };

    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        appState = AppState::MainMenu;
        return;
    }

    ImGui::Begin("Return Book");

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

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", message);    
    if (ImGui::Button("Back to Main Menu")) {
        appState = AppState::MainMenu;
        ImGui::End();
        return;
    }

    ImGui::End();
}

void AddBookUI(AppState& appState) {
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { appState = AppState::MainMenu; return; }
    if (curUser.getRole() != Role::LIBRARIAN) { appState = AppState::MainMenu; return; }

    static char isbn[32] = "";
    static char title[128] = "";
    static char authorID[16] = "";
    static char categoryID[16] = "";
    static int  totalCopies = 1;
    static char message[256] = "";

    ImGui::Begin("Add Book");

    ImGui::InputText("ISBN", isbn, IM_ARRAYSIZE(isbn));
    ImGui::InputText("Title", title, IM_ARRAYSIZE(title));
    ImGui::InputText("Author ID", authorID, IM_ARRAYSIZE(authorID));
    ImGui::InputText("Category ID", categoryID, IM_ARRAYSIZE(categoryID));
    ImGui::InputInt("Total Copies", &totalCopies);
    if (totalCopies < 1) totalCopies = 1;

    if (ImGui::Button("Add Book")) {
        auto trim = [](std::string& s){
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
            try { aID = std::stoi(sAuth); cID = std::stoi(sCat); }
            catch (...) { std::snprintf(message, sizeof(message), "Author/Category ID must be numbers."); goto after; }

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
    ImGui::SameLine(0, 12);
    if (ImGui::Button("Back to Main Menu")) { appState = AppState::MainMenu; ImGui::End(); return; }

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(1,1,0,1), "%s", message);
    ImGui::End();
}

void FindAuthorUI(AppState& appState) {
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { appState = AppState::MainMenu; return; }

    static char idBuf[32] = "";
    static char message[256] = "";
    static std::string foundID, foundName, foundBio;

    ImGui::Begin("Find Author");

    ImGui::InputText("Author ID", idBuf, IM_ARRAYSIZE(idBuf));

    if (ImGui::Button("Search")) {
        auto trim = [](std::string& s){
            auto issp = [](unsigned char c){ return std::isspace(c); };
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
            s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
        };

        std::string target = idBuf;
        trim(target);
        foundID.clear(); foundName.clear(); foundBio.clear();

        if (target.empty()) {
            std::snprintf(message, sizeof(message), "Please enter Author ID.");
        } else {
            std::ifstream file("../data/authors.csv");   
            if (!file.is_open()) {
                std::snprintf(message, sizeof(message), "Cannot open ../data/authors.csv");
            } else {
                std::string line;
                bool ok = false;

                if (std::getline(file, line)) { /* bỏ qua header */ }

                while (std::getline(file, line)) {
                    auto cols = CSVHandler::parseCSVLine(line);
                    if (cols.size() < 3) continue;

                    std::string id  = cols[0];
                    std::string name= cols[1];
                    std::string bio = cols[2];
                    trim(id); trim(name); trim(bio);

                    if (id == target) {
                        foundID = id; foundName = name; foundBio = bio;
                        ok = true; break;
                    }
                }
                file.close();

                if (ok) std::snprintf(message, sizeof(message), "Found author.");
                else    std::snprintf(message, sizeof(message), "Author ID '%s' not found.", target.c_str());
            }
        }
    }

    ImGui::SameLine(0, 12);
    if (ImGui::Button("Back to Main Menu")) { appState = AppState::MainMenu; ImGui::End(); return; }

    ImGui::Separator();
    ImGui::TextColored(ImVec4(1,1,0,1), "%s", message);

    if (!foundName.empty()) {
        ImGui::Text("ID:    %s", foundID.c_str());
        ImGui::Text("Name:  %s", foundName.c_str());
        ImGui::TextWrapped("Bio:  %s", foundBio.c_str());
    }

    ImGui::End();
}

void FindCategoryUI(AppState& appState) {
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { appState = AppState::MainMenu; return; }

    static char idBuf[32] = "";
    static char message[256] = "";
    static std::string foundID, foundName, foundDesc;

    ImGui::Begin("Find Category");

    ImGui::InputText("Category ID", idBuf, IM_ARRAYSIZE(idBuf));

    if (ImGui::Button("Search")) {
        auto trim = [](std::string& s){
            auto issp = [](unsigned char c){ return std::isspace(c); };
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
            s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
        };

        std::string target = idBuf;
        trim(target);
        foundID.clear(); foundName.clear(); foundDesc.clear();

        if (target.empty()) {
            std::snprintf(message, sizeof(message), "Please enter Category ID.");
        } else {
            std::ifstream file("../data/categories.csv");   
            if (!file.is_open()) {
                std::snprintf(message, sizeof(message), "Cannot open ../data/categories.csv");
            } else {
                std::string line;
                bool ok = false;

                if (std::getline(file, line)) { /* skip header */ }

                while (std::getline(file, line)) {
                    auto cols = CSVHandler::parseCSVLine(line);
                    if (cols.size() < 3) continue;

                    std::string id   = cols[0];
                    std::string name = cols[1];
                    std::string desc = cols[2];
                    trim(id); trim(name); trim(desc);

                    if (id == target) {
                        foundID = id; foundName = name; foundDesc = desc;
                        ok = true; break;
                    }
                }
                file.close();

                if (ok) std::snprintf(message, sizeof(message), "Found category.");
                else    std::snprintf(message, sizeof(message), "Category ID '%s' not found.", target.c_str());
            }
        }
    }

    ImGui::SameLine(0, 12);
    if (ImGui::Button("Back to Main Menu")) { appState = AppState::MainMenu; ImGui::End(); return; }

    ImGui::Separator();
    ImGui::TextColored(ImVec4(1,1,0,1), "%s", message);

    if (!foundName.empty()) {
        ImGui::Text("ID:    %s", foundID.c_str());
        ImGui::Text("Name:  %s", foundName.c_str());
        ImGui::TextWrapped("Desc: %s", foundDesc.c_str());
    }

    ImGui::End();
}


void MainMenuUI(AppState &appState) {
    ImGui::Begin("Main Menu");

    string curUsername = curUser.getUserName();
    static char usernameBuf[64];
    strncpy(usernameBuf, curUsername.c_str(), sizeof(usernameBuf));
    usernameBuf[sizeof(usernameBuf) - 1] = '\0';

    ImGui::Text("USER: %s", usernameBuf);
    ImGui::Text("STATUS: %s", roleToString(curUser.getRole()));
    ImGui::SameLine(250);

    ImGui::BeginGroup();
    ImGui::TextColored(ImVec4(0.1f, 0.7f, 0.3f, 1.0f), "Welcome to Library!");
    ImGui::TextWrapped("If u dont know, this is an electronic library belongs to HCMUS. "
                       "Hope u'll have a good experience using this service :3");
    ImGui::EndGroup();

    ImGui::Separator();

    static int selectedItem = 0;
    static Role lastRole = curUser.getRole();
    if (curUser.getRole() != lastRole) { selectedItem = 0; lastRole = curUser.getRole(); }

    std::vector<const char*> actions = { "Borrow Book", "Return Book", "Search Book" , "Find Author", "Find Category"};
    if (curUser.getRole() == Role::LIBRARIAN) actions.push_back("Add Book");

    if (actions.empty()) selectedItem = 0;
    else if (selectedItem >= (int)actions.size()) selectedItem = (int)actions.size() - 1;

    const int count = (int)actions.size();
    const char* items[6];
    for (int i = 0; i < count; ++i) items[i] = actions[i];

    ImGui::Text("Choose Action:");
    ImGui::SameLine();
    ImGui::Combo("##ActionCombo", &selectedItem, items, count); 

    if (ImGui::Button("Confirm")) {
        const char* chosen = items[selectedItem];
        if      (strcmp(chosen, "Borrow Book") == 0) appState = AppState::BorrowBook;
        else if (strcmp(chosen, "Return Book") == 0) appState = AppState::ReturnBook;
        else if (strcmp(chosen, "Search Book") == 0) appState = AppState::SearchBook;
        else if (strcmp(chosen, "Find Author") == 0) appState = AppState::FindAuthor;
        else if (strcmp(chosen, "Find Category") == 0) appState = AppState::FindCategory;
        else if (strcmp(chosen, "Add Book") == 0)    appState = AppState::AddBook;
    }

    if (ImGui::Button("Log out")) {
        curUser = User();
        selectedItem = 0;
        appState = AppState::Login;
    }

    ImGui::End();
}

void RenderUI() {
    if (appState == AppState::Register) {
        RegisterUI(appState);
    } else if (appState == AppState::Login) {
        LoginUI(appState);
    } else if (appState == AppState::MainMenu) {
        MainMenuUI(appState);
    } else if (appState == AppState::SearchBook) {
        SearchBookUI(appState);
    } else if (appState == AppState::BorrowBook) {
        BorrowBookUI(appState);
    } else if (appState == AppState::ReturnBook) {
        ReturnBookUI(appState);
    } else if (appState == AppState::AddBook) {
        AddBookUI(appState);
    } else if (appState == AppState::FindAuthor) {
        FindAuthorUI(appState);
    } else if (appState == AppState::FindCategory) {
        FindCategoryUI(appState);
    }
}

int main(int, char**)
{
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    auto& mgr = LibraryManager::getInstance();
    mgr.loadBooksIntoLibrary();    
    mgr.loadMembersFromCSV(); 
    mgr.loadLoansFromCSV();

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, 100, 100, (int)(1280 * main_scale), (int)(800 * main_scale), nullptr, nullptr, wc.hInstance, nullptr);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_DeviceLost)
        {
            HRESULT hr = g_pd3dDevice->TestCooperativeLevel();
            if (hr == D3DERR_DEVICELOST)
            {
                ::Sleep(10);
                continue;
            }
            if (hr == D3DERR_DEVICENOTRESET)
                ResetDevice();
            g_DeviceLost = false;
        }

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        myStyle();
        ImGui::SetNextWindowSize(ImVec2(600, 400));
        // Start the GUI here//////////////////////////////
        ///////////////////////////////////////////////////
        RenderUI();
        ///////////////////////////////////////////////////
        ///////////////////////////////////////////////////

        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
        if (result == D3DERR_DEVICELOST)
            g_DeviceLost = true;
    }

    // Cleanup
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
