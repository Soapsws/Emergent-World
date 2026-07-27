#include "rlImGui.h"
#include "imgui.h"

#include "UI.hpp"
#include "sim_constants.hpp"

// RAII allocation & cleanup using rlImGui's simplified memory management

UI::UI(bool useDarkTheme, PlayerCamera& camera, int& maxCells, int& maxFood)
    : camera(camera), maxCells(maxCells), maxFood(maxFood) {
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
        if (ImGui::CollapsingHeader("User settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            float panSpeed = camera.panSpeed();
            if (ImGui::SliderFloat("Camera speed", &panSpeed, 0.5f, 8.0f)) {
                camera.setPanSpeed(panSpeed);
            }

            ImGui::SliderFloat("Zoom speed", &camera.zoomScale, 0.2f, 5.0f);
        }

        if (ImGui::CollapsingHeader("Entities", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::SliderInt("Maximum cells", &maxCells, 1, cells::MAX_CELLS);
            ImGui::SliderInt("Maximum food", &maxFood, 1, food::MAX_NATURAL_FOOD);
        }
    }
    ImGui::End();
}
