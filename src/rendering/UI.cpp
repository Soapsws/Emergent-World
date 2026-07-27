#include "rlImGui.h"
#include "imgui.h"

#include "UI.hpp"

// RAII allocation & cleanup using rlImGui's simplified memory management

UI::UI(bool useDarkTheme) {
    rlImGuiSetup(useDarkTheme);
}

UI::~UI() {
    rlImGuiShutdown();
}

void UI::Begin() {
    rlImGuiBegin();
}


void UI::End() {
    rlImGuiEnd();
}

void UI::Draw() {
    if (ImGui::Begin("UI")) {
        ImGui::Text("sample text");
        if(ImGui::Button("sample button")) {
            ;
        }
    }
    ImGui::End();
}
