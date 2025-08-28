#include "imgui.h"
#include "AppState.hpp"                   
#include "services/LibraryManager.hpp"
#include "patterns/strategy/TitleSearchStrategy.hpp"
#include "utils/CSVHandler.hpp"
#include <string>
#include <cstring>
#include <fstream>
#include <algorithm>

void FindCategoryUI(AppState& appState) {
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) { appState = AppState::MainMenu; return; }
 
    static char idBuf[32] = "";
    static char message[256] = "";
    static std::string foundID, foundName, foundDesc;
 
    ImGui::Begin("Find Category");
 
    ImGui::InputText("Category ID", idBuf, IM_ARRAYSIZE(idBuf));
 
    if (ImGui::Button("Search")) {
        auto trim = [](std::string& s){
            auto issp = [](unsigned char c){ return std::isspace(c); };
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c){ return !issp(c); }));
            s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c){ return !issp(c); }).base(), s.end());
        };
 
        std::string target = idBuf;
        trim(target);
        foundID.clear(); foundName.clear(); foundDesc.clear();
 
        if (target.empty()) {
            std::snprintf(message, sizeof(message), "Please enter Category ID.");
        } else {
            std::ifstream file("../data/categories.csv");   
            if (!file.is_open()) {
                std::snprintf(message, sizeof(message), "Cannot open ../data/categories.csv");
            } else {
                std::string line;
                bool ok = false;
 
                if (std::getline(file, line)) { /* skip header */ }
 
                while (std::getline(file, line)) {
                    auto cols = CSVHandler::parseCSVLine(line);
                    if (cols.size() < 3) continue;
 
                    std::string id   = cols[0];
                    std::string name = cols[1];
                    std::string desc = cols[2];
                    trim(id); trim(name); trim(desc);
 
                    if (id == target) {
                        foundID = id; foundName = name; foundDesc = desc;
                        ok = true; break;
                    }
                }
                file.close();
 
                if (ok) std::snprintf(message, sizeof(message), "Found category.");
                else    std::snprintf(message, sizeof(message), "Category ID '%s' not found.", target.c_str());
            }
        }
    }
 
    ImGui::SameLine(0, 12);
    if (ImGui::Button("Back to Main Menu")) { appState = AppState::MainMenu; ImGui::End(); return; }
 
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1,1,0,1), "%s", message);
 
    if (!foundName.empty()) {
        ImGui::Text("ID:    %s", foundID.c_str());
        ImGui::Text("Name:  %s", foundName.c_str());
        ImGui::TextWrapped("Desc: %s", foundDesc.c_str());
    }
 
    ImGui::End();
}