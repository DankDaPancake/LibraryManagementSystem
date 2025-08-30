#include "imgui.h"
#include "AppState.hpp"
#include "services/LibraryManager.hpp"
#include "core/User.hpp"
#include "core/Loan.hpp"

#include <string>
#include <vector>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <chrono>
#include <ctime>

extern User curUser;

static void ShowDate(const std::chrono::system_clock::time_point& tp,
                     const char* fmt = "%Y-%m-%d")
{
    if (tp.time_since_epoch().count() == 0) { ImGui::TextUnformatted("-"); return; }
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm tm {};
#if defined(_WIN32)
    localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif
    char buf[32];
    std::strftime(buf, sizeof(buf), fmt, &tm);
    ImGui::TextUnformatted(buf);
}
static void ShowDate(const std::string& s) { ImGui::TextUnformatted(s.c_str()); }

static bool BeginCenteredModal(const char* name, ImVec2 size)
{
    ImGuiViewport* vp = ImGui::GetMainViewport();
    ImVec2 pos;
    pos.x = vp->WorkPos.x + (vp->WorkSize.x - size.x) * 0.5f;
    pos.y = vp->WorkPos.y + (vp->WorkSize.y - size.y) * 0.5f;
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    return ImGui::BeginPopupModal(name, nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
}

static char sMid[64]  = "";
static char sIsbn[64] = "";
static char sMsg[256] = "";
static void ClearFields() { sMid[0] = sIsbn[0] = sMsg[0] = '\0'; }

void BorrowBookUI(AppState& appState)
{
    static std::string lastUserId;
    static char memberID[64] = "";
    static char isbn[64]     = "";
    static char message[256] = "";
    static int  dueDate      = 30; 

    if (lastUserId != curUser.getUserID()) {
        lastUserId = curUser.getUserID();
        memberID[0] = isbn[0] = message[0] = '\0';
    }
    auto trim = [](std::string& s){
        auto issp = [](unsigned char c){ return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
    };

    // ====== DASHBOARD title (chỉ thêm UI, không đổi logic) ======
    {
        ImGui::Dummy(ImVec2(0, 4));               // top padding nhỏ
        float old = ImGui::GetFont()->Scale;
        ImGui::GetFont()->Scale = old * 1.05f;    // nhỉnh hơn chút để giống tiêu đề
        ImGui::PushFont(ImGui::GetFont());
        ImGui::TextUnformatted("Dashboard");
        ImGui::PopFont();
        ImGui::GetFont()->Scale = old;
        ImGui::Dummy(ImVec2(0, 2));
        ImGui::Separator();                        // phân tách với phần nội dung bên dưới
        ImGui::Dummy(ImVec2(0, 6));
    }

    // ===== Hàng tiêu đề + 2 nút bên phải =====
    {
        const float btnW = 140.0f;
        const float gap  = 10.0f;
        ImGui::TextUnformatted("Books collection:");
        ImGui::SameLine();

        float keepX = ImGui::GetCursorPosX();
        float avail = ImGui::GetContentRegionAvail().x;
        ImGui::SetCursorPosX(keepX + avail - (btnW * 2.0f + gap));

        if (ImGui::Button("Borrow Book", ImVec2(btnW, 0))) {
            ImGui::OpenPopup("Borrow Book");
        }
        ImGui::SameLine(0.0f, gap);
        if (ImGui::Button("Return Book", ImVec2(btnW, 0))) {
            ClearFields();
            ImGui::OpenPopup("Return Book");
        }
    }

    if (ImGui::BeginTable("loan_table", 6,
        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
    {
        ImGui::TableSetupColumn("Loan ID");
        ImGui::TableSetupColumn("Book ISBN");
        ImGui::TableSetupColumn("Book Title");
        ImGui::TableSetupColumn("Member ID");
        ImGui::TableSetupColumn("Borrow Date");
        ImGui::TableSetupColumn("Status");
        ImGui::TableHeadersRow();

        auto& manager   = LibraryManager::getInstance();
        const auto& ls  = manager.getLoans();

        for (const auto& loan : ls) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0); ImGui::TextUnformatted(loan->getLoanID().c_str());
            ImGui::TableSetColumnIndex(1); ImGui::TextUnformatted(loan->getBookISBN().c_str());

            ImGui::TableSetColumnIndex(2);
            if (Book* b = manager.findBook(loan->getBookISBN()))
                ImGui::TextUnformatted(b->getTitle().c_str());
            else
                ImGui::TextDisabled("Unknown / Not found");

            ImGui::TableSetColumnIndex(3); ImGui::TextUnformatted(loan->getMemberID().c_str());

            ImGui::TableSetColumnIndex(4); ShowDate(loan->getBorrowDate());
            ImGui::TableSetColumnIndex(5);

            bool overdue = loan->isOverdue();
            int  days    = loan->getDaysOverdue();

            std::string st = loan->loanStatusToString(loan->getStatus());

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

    // ===== Popup: Borrow Book =====
    if (BeginCenteredModal("Borrow Book", ImVec2(520, 220))) {
        ImGui::TextUnformatted("Enter Borrow Book Details:");
        ImGui::InputText("Member ID", memberID, IM_ARRAYSIZE(memberID));
        ImGui::InputText("Book ISBN",  isbn,     IM_ARRAYSIZE(isbn));
        ImGui::InputInt ("Borrow Date", &dueDate); // nếu không dùng có thể ẩn đi

        if (ImGui::Button("Borrow")) {
            std::string mid = memberID, code = isbn; trim(mid); trim(code);
            if (mid.empty() || code.empty()) {
                std::snprintf(message, sizeof(message), "Please fill in both Member ID and ISBN.");
            } else {
                auto& manager = LibraryManager::getInstance();
                if (manager.borrowBook(mid, code)) {
                    manager.saveBooksNewInfo();
                    std::snprintf(message, sizeof(message), "Book borrowed successfully!");
                    isbn[0] = '\0';
                    ImGui::CloseCurrentPopup();
                } else {
                    if (Book* b = manager.findBook(code)) {
                        if (b->getAvailableCopies() <= 0)
                            std::snprintf(message, sizeof(message), "No copies available.");
                        else
                            std::snprintf(message, sizeof(message), "Not available or already borrowed.");
                    } else {
                        std::snprintf(message, sizeof(message), "ISBN not found.");
                    }
                }
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) ImGui::CloseCurrentPopup();

        if (message[0]) {
            ImGui::Dummy(ImVec2(0, 6));
            ImGui::TextColored(ImVec4(1,0,0,1), "%s", message);
        }
        ImGui::EndPopup();
    }

    // ===== Popup: Return Book =====
    if (BeginCenteredModal("Return Book", ImVec2(680, 300))) {
        auto& manager = LibraryManager::getInstance();

        ImGui::TextUnformatted("Member ID");
        ImGui::InputText("##ret_mid", sMid, IM_ARRAYSIZE(sMid));
        ImGui::TextUnformatted("Book ISBN");
        ImGui::InputText("##ret_isbn", sIsbn, IM_ARRAYSIZE(sIsbn));

        ImGui::Dummy(ImVec2(0, 10));
        if (sMsg[0]) ImGui::TextColored(ImVec4(1,1,0,1), "%s", sMsg);

        ImGui::Dummy(ImVec2(0, 8));
        const float btnW = 110.0f, btnH = 28.0f;

        if (ImGui::Button("Return", ImVec2(btnW, btnH))) {
            std::string mid = sMid, code = sIsbn; trim(mid); trim(code);
            if (mid.empty() || code.empty()) {
                std::snprintf(sMsg, sizeof(sMsg), "Please fill in both Member ID and ISBN.");
            } else {
                if (manager.returnBook(mid, code)) {
                    manager.saveBooksNewInfo();
                    std::snprintf(sMsg, sizeof(sMsg), "Returned successfully!");
                    sIsbn[0] = '\0';
                } else {
                    std::snprintf(sMsg, sizeof(sMsg),
                                  "Return failed. No active loan for this member & ISBN.");
                }
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear##ret", ImVec2(btnW, btnH))) ClearFields();
        ImGui::SameLine();
        if (ImGui::Button("Close##ret", ImVec2(btnW, btnH))) { ClearFields(); ImGui::CloseCurrentPopup(); }

        ImGui::EndPopup();
    }
}
