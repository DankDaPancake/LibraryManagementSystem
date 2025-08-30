#include "imgui.h"
#include "AppState.hpp"
#include "services/AuthenticateManager.hpp"
#include "core/User.hpp"
#include <algorithm>
#include <cstring>

extern User curUser;

static void DrawFocusStripe(float pad_y, float thickness, ImU32 col,
                            float dash = 8.0f, float gap = 5.0f)
{
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 a = ImGui::GetItemRectMin();
    ImVec2 b = ImGui::GetItemRectMax();
    float y1 = b.y + pad_y, y2 = y1 + thickness;
    for (float x = a.x; x < b.x; x += (dash + gap)) {
        float x2 = x + dash; if (x2 > b.x) x2 = b.x;
        dl->AddRectFilled(ImVec2(x, y1), ImVec2(x2, y2), col, thickness * 0.5f);
    }
}

void LoginUI(AppState& appState)
{
    ImGuiViewport* vp = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos);
    ImGui::SetNextWindowSize(vp->WorkSize);
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                             ImGuiWindowFlags_NoMove     | ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    static char username[64] = "";
    static char password[64] = "";
    static bool loginFailed  = false;

    if (!ImGui::Begin("LMS/Login", nullptr, flags)) { ImGui::End(); return; }

    const float topPad    = 36.0f;
    const float bottomGap = 110.0f;                 
    const float availX    = ImGui::GetContentRegionAvail().x;
    const float availY    = ImGui::GetContentRegionAvail().y;

    const float cardW  = std::clamp(availX * 0.70f, 540.0f, 760.0f);
    const float cardX  = (availX - cardW) * 0.5f;
    const float wantH  = 520.0f;                    
    const float maxH   = std::max(360.0f, availY - topPad - bottomGap);
    const float cardH  = std::min(wantH, maxH);

    ImGui::Dummy(ImVec2(0, topPad));
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + cardX);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.95f, 0.96f, 0.98f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border,  ImVec4(0.08f, 0.22f, 0.28f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text,    ImVec4(0,0,0,1));
    ImGui::PushStyleVar  (ImGuiStyleVar_ChildRounding, 14.0f);
    ImGui::PushStyleVar  (ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 20.0f));

    if (ImGui::BeginChild("login_card", ImVec2(cardW, cardH), true, ImGuiWindowFlags_NoScrollbar)) {

        ImGui::Dummy(ImVec2(0, 6));
        float s = ImGui::GetFont()->Scale;
        ImGui::GetFont()->Scale = s * 1.35f; ImGui::PushFont(ImGui::GetFont());
        ImGui::TextUnformatted("fit@hcmus");
        ImGui::PopFont(); ImGui::GetFont()->Scale = s;
        ImGui::TextUnformatted("VNUHCM - UNIVERSITY OF SCIENCE");
        ImGui::TextUnformatted("FACULTY OF INFORMATION TECHNOLOGY");
        ImGui::Dummy(ImVec2(0, 8)); ImGui::Separator(); ImGui::Dummy(ImVec2(0, 6));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_FrameBg,        ImVec4(1,1,1,1));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.96f,0.98f,1.0f,1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  ImVec4(0.93f,0.97f,1.0f,1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border,         ImVec4(0.12f,0.44f,0.66f,1.0f));

        bool enterLogin = false;

        ImGui::TextUnformatted("Username");
        ImGui::SetNextItemWidth(-FLT_MIN);
        enterLogin = ImGui::InputText("##login_user", username, IM_ARRAYSIZE(username),
                                      ImGuiInputTextFlags_EnterReturnsTrue);
        if (ImGui::IsItemHovered()) DrawFocusStripe(4.0f, 3.0f, IM_COL32(60,140,200,180));
        if (ImGui::IsItemActive())  DrawFocusStripe(4.0f, 3.0f, IM_COL32(30,110,180,255));

        ImGui::Dummy(ImVec2(0, 10));
        ImGui::TextUnformatted("Password");
        ImGui::SetNextItemWidth(-FLT_MIN);
        enterLogin |= ImGui::InputText("##login_pass", password, IM_ARRAYSIZE(password),
                                       ImGuiInputTextFlags_Password | ImGuiInputTextFlags_EnterReturnsTrue);
        if (ImGui::IsItemHovered()) DrawFocusStripe(4.0f, 3.0f, IM_COL32(60,140,200,180));
        if (ImGui::IsItemActive())  DrawFocusStripe(4.0f, 3.0f, IM_COL32(30,110,180,255));

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar(2);

        ImGui::Dummy(ImVec2(0, 8));
        {
            const char* hint = "If you don't have account, pls use \"Register\" button below!";
            float tw = ImGui::CalcTextSize(hint).x;
            float cx = (ImGui::GetContentRegionAvail().x - tw) * 0.5f;
            float keepX = ImGui::GetCursorPosX();
            ImGui::SetCursorPosX(keepX + (cx > 0 ? cx : 0));
            ImGui::TextUnformatted(hint);
            ImGui::SetCursorPosX(keepX);
        }

        ImGui::Dummy(ImVec2(0, 48.0f));                
        const float padX = 12.0f, gap = 12.0f, btnH = 44.0f;
        float contentW = ImGui::GetContentRegionAvail().x;
        float btnW     = (contentW - padX * 2.0f - gap) * 0.5f;

        ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.10f,0.35f,0.50f,1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.12f,0.46f,0.66f,1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.08f,0.30f,0.45f,1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text,          ImVec4(1,1,1,1));

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padX);
        if (ImGui::Button("Register", ImVec2(btnW, btnH))) {
            loginFailed = false;
            appState = AppState::Register;
            ImGui::PopStyleColor(4);
            ImGui::EndChild(); ImGui::PopStyleVar(2); ImGui::PopStyleColor(3); ImGui::End();
            return;
        }
        ImGui::SameLine(0, gap);
        bool doLogin = ImGui::Button("Sign in", ImVec2(btnW, btnH)) || enterLogin;
        ImGui::PopStyleColor(4);

        ImGui::Dummy(ImVec2(0, 48.0f));

        if (doLogin) {
            AuthenticateManager auth;
            if (User* u = auth.loginUser(username, password)) {
                curUser = *u; delete u;
                loginFailed = false;
                appState = AppState::MainMenu;
                ImGui::EndChild(); ImGui::PopStyleVar(2); ImGui::PopStyleColor(3); ImGui::End();
                return;
            } else loginFailed = true;
        }

        if (loginFailed)
            ImGui::TextColored(ImVec4(0.85f,0.20f,0.20f,1.0f), "Invalid username or password.");
    }
    ImGui::EndChild();

    ImGui::PopStyleVar(2);   
    ImGui::PopStyleColor(3); 
    ImGui::End();
}
