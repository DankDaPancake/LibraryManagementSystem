#include "imgui.h"
#include "AppState.hpp"
#include "services/LibraryManager.hpp"
#include "core/Loan.hpp"
#include "core/Book.hpp"

#include <string>
#include <chrono>
#include <ctime>
#include <cstdio>

static std::string FormatYMD(const std::chrono::system_clock::time_point& tp)
{
    using namespace std::chrono;
    if (tp.time_since_epoch().count() == 0) return "-";

    std::time_t t = system_clock::to_time_t(tp);
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
                  tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    return std::string(buf);
}

void ListLoanUI(AppState& appState)
{
    auto& manager = LibraryManager::getInstance();

    ImGui::TextUnformatted("Loans");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 4));

    ImGuiTableFlags flags =
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_Resizable |
        ImGuiTableFlags_SizingStretchProp |
        ImGuiTableFlags_ScrollY;

    const float tableMinH = ImGui::GetContentRegionAvail().y;

    if (ImGui::BeginTable("loan_list_table", 8, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Loan ID",     ImGuiTableColumnFlags_WidthFixed,   90.0f);
        ImGui::TableSetupColumn("Book ISBN",   ImGuiTableColumnFlags_WidthFixed,  130.0f);
        ImGui::TableSetupColumn("Book Title",  ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Member ID",   ImGuiTableColumnFlags_WidthFixed,   80.0f);
        ImGui::TableSetupColumn("Borrow Date", ImGuiTableColumnFlags_WidthFixed,  110.0f);
        ImGui::TableSetupColumn("Due Date",    ImGuiTableColumnFlags_WidthFixed,  110.0f);
        ImGui::TableSetupColumn("Return Date", ImGuiTableColumnFlags_WidthFixed,  110.0f);
        ImGui::TableSetupColumn("Status",      ImGuiTableColumnFlags_WidthFixed,   90.0f);
        ImGui::TableHeadersRow();

        const auto& loans = manager.getLoans();
        for (const auto& ln : loans)
        {
            if (!ln) continue;

            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted(ln->getLoanID().c_str());

            ImGui::TableSetColumnIndex(1);
            ImGui::TextUnformatted(ln->getBookISBN().c_str());

            ImGui::TableSetColumnIndex(2);
            if (Book* b = manager.findBook(ln->getBookISBN()))
                ImGui::TextUnformatted(b->getTitle().c_str());
            else
                ImGui::TextDisabled("Unknown / Not found");

            ImGui::TableSetColumnIndex(3);
            ImGui::TextUnformatted(ln->getMemberID().c_str());

            ImGui::TableSetColumnIndex(4);
            ImGui::TextUnformatted(FormatYMD(ln->getBorrowDate()).c_str());

            ImGui::TableSetColumnIndex(5);
            ImGui::TextUnformatted(FormatYMD(ln->getDueDate()).c_str());

            ImGui::TableSetColumnIndex(6);
            ImGui::TextUnformatted(FormatYMD(ln->getReturnDate()).c_str());

            ImGui::TableSetColumnIndex(7);
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
