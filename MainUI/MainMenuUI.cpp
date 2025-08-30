#include "imgui.h"
#include "AppState.hpp"
#include "core/User.hpp"
#include "Screens.hpp"
#include <vector>
#include <cstring>
#include <string>

extern User curUser;
const char* roleToString(Role);

static int AppStateToIndex(AppState s, bool isLibrarian) {
    if (!isLibrarian) {
        switch (s) {
            case AppState::SearchBook:   return 0;
            case AppState::MemberManage: return 1;
            case AppState::Notification: return 2;
            default:                     return 0;
        }
    }

    switch (s) {
        case AppState::BorrowBook:   return 0;
        case AppState::SearchBook:   return 1;
        case AppState::AddBook:      return 2;
        case AppState::ListMember:   return 3;
        case AppState::ListLoan:     return 4;
        case AppState::Notification: return 5;
        default:                     return 0;
    }
}

static AppState IndexToAppState(int idx, bool isLibrarian) {
    if (!isLibrarian) {
        if (idx == 0) return AppState::SearchBook;   // Browse
        if (idx == 1) return AppState::MemberManage;
        if (idx == 2) return AppState::Notification;
        return AppState::SearchBook;
    }

    if (idx == 0) return AppState::BorrowBook;
    if (idx == 1) return AppState::SearchBook;
    if (idx == 2) return AppState::AddBook;
    if (idx == 3) return AppState::ListMember;
    if (idx == 4) return AppState::ListLoan;
    if (idx == 5) return AppState::Notification;
    return AppState::BorrowBook;
}

static bool NavButton(const char* label, bool selected, float width) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 10));
    if (selected) {
        ImGui::PushStyleColor(ImGuiCol_Button,        ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
    }
    bool clicked = ImGui::Button(label, ImVec2(width, 0));
    if (selected) ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(2);
    return clicked;
}

inline void LmsShellUI(AppState& appState) {
    ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove     | ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    static float sidebarWidth = 260.0f;
    static Role lastRole = curUser.getRole();
    const bool isLibrarian = (curUser.getRole() == Role::LIBRARIAN);

    if (!isLibrarian && appState == AppState::BorrowBook)
        appState = AppState::SearchBook;

    std::vector<const char*> actions;
    if (isLibrarian) {
        actions = { "Dashboard", "Browse", "Book Manage", "Member Manage", "Loan Manage" , "Notification"};
    } else {
        actions = { "Browse", "Member Manage", "Notification"}; 
    }

    static int selected = AppStateToIndex(appState, isLibrarian);
    if (curUser.getRole() != lastRole) {
        lastRole = curUser.getRole();
        selected = AppStateToIndex(appState, isLibrarian);
    }
    if (selected >= (int)actions.size()) selected = (int)actions.size() - 1;

    if (ImGui::Begin("LMS - Shell", nullptr, flags)) {
        ImGui::BeginChild("sidebar", ImVec2(sidebarWidth, 0), true, ImGuiWindowFlags_NoScrollbar);
        {
            ImGui::Dummy(ImVec2(0, 6));
            ImGui::TextColored(ImVec4(0.20f,0.60f,1.0f,1.0f), "LMS");
            ImGui::TextDisabled("HCMUS Electronic Library");
            ImGui::Dummy(ImVec2(0, 6)); ImGui::Separator(); ImGui::Dummy(ImVec2(0, 6));

            for (int i = 0; i < (int)actions.size(); ++i) {
                if (NavButton(actions[i], selected == i, sidebarWidth - 24)) {
                    selected = i;
                }
                ImGui::Dummy(ImVec2(0, 4));
            }

            ImGui::Dummy(ImVec2(0, 8)); ImGui::Separator(); ImGui::Dummy(ImVec2(0, 8));
            if (ImGui::Button("Log out", ImVec2(sidebarWidth - 24, 0))) {
                curUser = User();
                appState = AppState::Login;
                ImGui::EndChild();
                ImGui::End();
                return;
            }

            ImGui::Dummy(ImVec2(0, 6));
            ImGui::TextDisabled("Sidebar width"); ImGui::SameLine();
            ImGui::SliderFloat("##sbw", &sidebarWidth, 220.0f, 380.0f, "%.0f px");
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("content", ImVec2(0, 0), false, ImGuiWindowFlags_None);
        {
            ImGui::BeginChild("topbar", ImVec2(0, 44), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            {
                const std::string user  = curUser.getUserName();
                const char* roleCStr    = roleToString(curUser.getRole());
                const std::string label = user + "  (" + roleCStr + ")";

                const float textW   = ImGui::CalcTextSize(label.c_str()).x;
                const float availW  = ImGui::GetContentRegionAvail().x;
                const float barH    = 44.0f;
                const float textH   = ImGui::GetTextLineHeight();
                const float topPadY = (barH - textH) * 0.5f;

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + topPadY);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + availW - textW);
                ImGui::AlignTextToFramePadding();
                ImGui::TextUnformatted(label.c_str());
            }
            ImGui::EndChild();
            ImGui::Separator();

            ImGui::BeginChild("content_body", ImVec2(0, 0), false, ImGuiWindowFlags_None);
            AppState inner = IndexToAppState(selected, isLibrarian);
            switch (inner) {
                case AppState::BorrowBook:   BorrowBookUI(appState);   break;
                case AppState::AddBook:      AddBookUI(appState);      break;
                case AppState::ListMember:   ListMemberUI(appState);   break;
                case AppState::ListLoan:     ListLoanUI(appState);     break;
                case AppState::Notification: NotificationUI(appState); break;
                case AppState::SearchBook:   SearchBookUI(appState);   break;
                case AppState::MemberManage: MemberManageUI(appState); break;

                default:
                    ImGui::TextDisabled("Select a feature from the left sidebar.");
                    break;
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void MainMenuUI(AppState& appState) {
    LmsShellUI(appState);
}
