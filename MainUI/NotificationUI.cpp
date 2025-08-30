#include "imgui.h"
#include "AppState.hpp"
#include <algorithm>

void NotificationUI(AppState& appState)
{
    ImGui::TextUnformatted("Notifications");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 8));
}
