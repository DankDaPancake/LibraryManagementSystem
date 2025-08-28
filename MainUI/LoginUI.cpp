#include "imgui.h"
#include "AppState.hpp"                 
#include "services/AuthenticateManager.hpp"   
#include <string>
#include <cstring>
#include "core/User.hpp"

extern User curUser;

void LoginUI(AppState &appState) {
    static char usernameInput[64] = "";
    static char passwordInput[64] = "";
    Role roleInput;
    bool loginSuccess = false;
    bool loginFailed = false;

    ImGui::Begin("Library Management System");

    ImGui::InputText("Username", usernameInput, IM_ARRAYSIZE(usernameInput));
    ImGui::InputText("Password", passwordInput, IM_ARRAYSIZE(passwordInput), ImGuiInputTextFlags_Password);

    if (ImGui::Button("Login")) {
        AuthenticateManager auth;
        User* user = auth.loginUser(usernameInput, passwordInput);

        if (user != nullptr) {
            curUser = *user;
            delete user;
            appState = AppState::MainMenu;
            ImGui::End();   
            return;         
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
            ImGui::End();   // <-- thêm
            return;         // <-- thêm
        }
    }

    ImGui::End();
}