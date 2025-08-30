#include "imgui.h"
#include "AppState.hpp"
#include "services/LibraryManager.hpp"
#include "core/User.hpp"
#include "core/Member.hpp"
#include "core/Loan.hpp"

extern User curUser;

void ListMemberUI(AppState& appState)
{
    auto& manager = LibraryManager::getInstance();

    ImGui::Dummy(ImVec2(0, 6));
    ImGui::TextUnformatted("Members");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));

    ImGuiTableFlags flags =
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_SizingStretchProp;

    const float tableMinH   = ImGui::GetContentRegionAvail().y;

    const float colBorrowW  = 140.0f;        
    const float wMemberID   = 0.80f;         
    const float wUsername   = 1.20f;        

    if (ImGui::BeginTable("members_table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Member ID",    ImGuiTableColumnFlags_WidthStretch, wMemberID);
        ImGui::TableSetupColumn("Username",     ImGuiTableColumnFlags_WidthStretch, wUsername);
        ImGui::TableSetupColumn("Total Borrow", ImGuiTableColumnFlags_WidthFixed,   colBorrowW);

        ImGui::TableHeadersRow();

        const auto& members = manager.getMembers();
        const auto& loans   = manager.getLoans();

        for (const Member* m : members)
        {
            if (!m) continue;

            int totalBorrow = 0;
            for (const Loan* ln : loans)
                if (ln && ln->getMemberID() == m->getUserID())
                    ++totalBorrow;

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::TextUnformatted(m->getUserID().c_str());
            ImGui::TableSetColumnIndex(1); ImGui::TextUnformatted(m->getUserName().c_str());
            ImGui::TableSetColumnIndex(2); ImGui::Text("%d", totalBorrow);
        }

        ImGui::EndTable();
    }
}
