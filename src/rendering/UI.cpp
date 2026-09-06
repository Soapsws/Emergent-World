#include "rlImGui.h"
#include "imgui.h"

#include "UI.hpp"
#include "sim_constants.hpp"

#include <cmath>

// RAII allocation & cleanup using rlImGui's simplified memory management

UI::UI(bool useDarkTheme, PlayerCamera& camera, int& maxCells, int& maxFood)
    : camera(camera), maxCells(maxCells), maxFood(maxFood) {
    rlImGuiSetup(useDarkTheme);
    ImPlot::CreateContext();
}

UI::~UI() {
    ImPlot::DestroyContext();
    rlImGuiShutdown();
}

void UI::Begin() {
    rlImGuiBegin();
}

void UI::TrackStatefulEntity(std::vector<float> data) {
    // AggregateData's observation layout is:
    // food distance, food angle sin/cos,
    // predator distance, predator angle sin/cos,
    // left/center/right whiskers, health, energy, hunger.
    if (data.size() < 12) return;

    const float foodAngle = std::atan2(data[1], data[2]);
    const float predatorAngle = std::atan2(data[4], data[5]);

    ImGui::SetNextWindowSize(ImVec2(430.0f, 360.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(
        ImVec2(300.0f, 240.0f),
        ImVec2(FLT_MAX, FLT_MAX)
    );
    if (ImGui::Begin("Stateful Entity")) {
        if (ImGui::BeginTable("proximity", 3,
                ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                ImGuiTableFlags_SizingStretchProp)) {
            ImGui::TableSetupColumn("Target");
            ImGui::TableSetupColumn("Distance");
            ImGui::TableSetupColumn("Angle");
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Nearest food");
            ImGui::TableNextColumn();
            ImGui::Text("%.3f", data[0]);
            ImGui::TableNextColumn();
            ImGui::Text("%.3f rad", foodAngle);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Nearest predator");
            ImGui::TableNextColumn();
            ImGui::Text("%.3f", data[3]);
            ImGui::TableNextColumn();
            ImGui::Text("%.3f rad", predatorAngle);

            ImGui::EndTable();
        }

        ImGui::SeparatorText("Whisker proximity");
        if (ImGui::BeginTable("whiskers", 3,
                ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                ImGuiTableFlags_SizingStretchProp)) {
            ImGui::TableSetupColumn("Left");
            ImGui::TableSetupColumn("Center");
            ImGui::TableSetupColumn("Right");
            ImGui::TableHeadersRow();
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("%.3f", data[6]);
            ImGui::TableNextColumn();
            ImGui::Text("%.3f", data[7]);
            ImGui::TableNextColumn();
            ImGui::Text("%.3f", data[8]);
            ImGui::EndTable();
        }

        ImGui::SeparatorText("Vitals");
        ImGui::Text("Health: %.3f", data[9]);
        ImGui::Text("Energy: %.3f", data[10]);
        ImGui::Text("Hunger: %.3f", data[11]);
    }
    ImGui::End();
}


void UI::End() {
    rlImGuiEnd();
}

void UI::Draw() {
    if (ImGui::Begin("UI")) {
        if (ImGui::CollapsingHeader("User settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            float panSpeed = camera.PanSpeed();
            if (ImGui::SliderFloat("Camera speed", &panSpeed, 0.5f, 8.0f)) {
                camera.SetPanSpeed(panSpeed);
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
