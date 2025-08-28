#include "imgui.h"
#include "AppState.hpp"                 
#include "services/AuthenticateManager.hpp"    
#include <string>
#include <cstring>                           

void RegisterUI(AppState &appState) {
    static std::string newUsername = "";
    static std::string newPassword = "";

    static bool registerSuccess = false;
    static bool registerFailed  = false;

    static char usernameBuf[64];
    std::strncpy(usernameBuf, newUsername.c_str(), sizeof(usernameBuf));
    usernameBuf[sizeof(usernameBuf) - 1] = '\0';

    static char passwordBuf[64];
    std::strncpy(passwordBuf, newPassword.c_str(), sizeof(passwordBuf));
    passwordBuf[sizeof(passwordBuf) - 1] = '\0';

    ImGui::Begin("Register");

    ImGui::Text("Register form here...");
    if (ImGui::InputText("New Username", usernameBuf, IM_ARRAYSIZE(usernameBuf))) {
        newUsername = usernameBuf;
    }
    if (ImGui::InputText("New Password", passwordBuf, IM_ARRAYSIZE(passwordBuf),
                         ImGuiInputTextFlags_Password)) {
        newPassword = passwordBuf;
    }

    struct RoleItem { const char* name; Role value; };
    static RoleItem roleItems[] = {
        {"MEMBER",    Role::MEMBER},
        {"LIBRARIAN", Role::LIBRARIAN},
    };
    static int currentRoleIndex = 0;
    const char* roleNames[] = { roleItems[0].name, roleItems[1].name };
    const int   roleCount   = 2;

    ImGui::Combo("Role", &currentRoleIndex, roleNames, roleCount);
    Role chosenRole = roleItems[currentRoleIndex].value;

    if (ImGui::Button("Register")) {
        AuthenticateManager auth;
        if (auth.registerUser(newUsername, newPassword, chosenRole)) {
            registerSuccess = true;
            registerFailed  = false;
            appState        = AppState::Login;
        } else {
            registerSuccess = false;
            registerFailed  = true;
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