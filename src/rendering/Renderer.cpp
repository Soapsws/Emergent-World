#include <raylib.h>

#include "Camera.hpp"
#include "Renderer.hpp"
#include "sim_constants.hpp"

#include "CellPool.hpp"
#include "FoodPool.hpp"

// another usage of initializer list
Renderer::Renderer() : pcam(settings::SCREEN_WIDTH, settings::SCREEN_HEIGHT) {}

void Renderer::RenderCells(const CellPool& cellPool) {
    for (int i = 0; i < static_cast<int>(cellPool.active.size()); ++i) {
        if (cellPool.active[i]) {
            DrawCircleV(cellPool.transform[i].position, cellPool.radius[i], colors::GetNeonColor(colors::NEON_AZURE));
        }
    }
}

void Renderer::RenderFood(const FoodPool& foodPool) {
    for (int i = 0; i < static_cast<int>(foodPool.active.size()); ++i) {
        if (foodPool.active[i]) {
            DrawCircleV(foodPool.transform[i].position, foodPool.radius[i], colors::GetNeonColor(colors::NEON_PINK));
        }
    }
}
