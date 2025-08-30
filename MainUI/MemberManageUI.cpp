// MemberManageUI.cpp
#include "imgui.h"
#include "AppState.hpp"
#include "services/LibraryManager.hpp"
#include "core/Book.hpp"
#include "core/Loan.hpp"
#include "core/User.hpp"
#include <chrono>
#include <cfloat>   

extern User curUser;

void MemberManageUI(AppState& appState)
{
    auto& manager = LibraryManager::getInstance();

    ImGui::TextUnformatted("Your Borrowed Books");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 4));

    ImGuiTableFlags flags =
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_SizingStretchProp |
        ImGuiTableFlags_ScrollY;

    const float tableH = ImGui::GetContentRegionAvail().y;

    if (ImGui::BeginTable("member_loans_table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Book ISBN",  ImGuiTableColumnFlags_WidthFixed,   150.0f);
        ImGui::TableSetupColumn("Book Title", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Status",     ImGuiTableColumnFlags_WidthFixed,   110.0f);
        ImGui::TableHeadersRow();

        const auto& loans = manager.getLoans();
        for (const auto &ln : loans)
        {
            if (!ln) continue;
            if (ln->getMemberID() != curUser.getUserID()) continue;


            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(ln->getBookISBN().c_str());

            ImGui::TableSetColumnIndex(1);
            if (Book* b = manager.findBook(ln->getBookISBN()))
                ImGui::TextUnformatted(b->getTitle().c_str());
            else
                ImGui::TextDisabled("Unknown");
            ImGui::TableSetColumnIndex(2);
            bool overdue = ln->isOverdue();
            int  days    = ln->getDaysOverdue();

            std::string st = ln->loanStatusToString(ln->getStatus());

            if (st == "RETURNED") {
                ImGui::TextColored(ImVec4(0.25f,0.75f,0.35f,1.0f), "RETURNED");
            } else if (overdue) {
                if (days > 0) ImGui::TextColored(ImVec4(0.85f,0.30f,0.25f,1.0f), "OVERDUE (%d)", days);
                else          ImGui::TextColored(ImVec4(0.85f,0.30f,0.25f,1.0f), "OVERDUE");
            } else {
                ImGui::TextColored(ImVec4(0.30f,0.70f,0.40f,1.0f), "ACTIVE");
            }
        }

        ImGui::EndTable();
    }
}
