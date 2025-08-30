#include "imgui.h"
#include "AppState.hpp"
#include <algorithm>  

void NotificationUI(AppState& appState)
{
    ImGui::TextUnformatted("Notifications");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));

    const ImVec2 avail = ImGui::GetContentRegionAvail();
    const float cardw  = (avail.x <= 0.f) ? 600.f : std::clamp(avail.x * 0.75f, 520.0f, 900.0f);
    const float cardh  = (avail.y <= 0.f) ? 260.f : std::clamp(avail.y * 0.50f, 200.0f, 420.0f);
    const float startX = (avail.x - cardw) * 0.5f;

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.95f, 0.96f, 0.98f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 12.0f);

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (startX > 0 ? startX : 0));
    if (ImGui::BeginChild("notif_card", ImVec2(cardw, cardh), true, ImGuiWindowFlags_NoScrollbar))
    {
        const char* emptyMsg = "No notifications yet.";
        const ImVec2 msgSz   = ImGui::CalcTextSize(emptyMsg);

        const float yPad = (cardh - msgSz.y) * 0.5f;
        const float xPad = (cardw - msgSz.x) * 0.5f;

        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (yPad > 0 ? yPad : 0));
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (xPad > 0 ? xPad : 0));
        ImGui::TextDisabled("%s", emptyMsg);
    }
    ImGui::EndChild();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}
