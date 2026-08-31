#include <raylib.h>

#include "Camera.hpp"
#include "Renderer.hpp"
#include "sim_constants.hpp"



// another usage of initializer list
Renderer::Renderer() : pcam(settings::SCREEN_WIDTH, settings::SCREEN_HEIGHT) { }

// RAII to have the acquired reference (the rendertexture) be destroyed when the renderer is destroyed
Renderer::~Renderer() {
    if (isInitialized) UnloadRenderTexture(wallsTexture);
}

void Renderer::ManualInit() {
    wallsTexture = LoadRenderTexture(settings::WORLD_WIDTH, settings::WORLD_HEIGHT);
    isInitialized = true;
}

void Renderer::RenderCells(const CellPool& cellPool) {
    for (int i = 0; i < static_cast<int>(cellPool.active.size()); ++i) {
        if (cellPool.active[i]) {
            DrawCircleV(cellPool.state[i].transform.position, cellPool.radius[i], colors::GetNeonColor(colors::NEON_AZURE));
        }
    }
}

void Renderer::RenderFood(const FoodPool& foodPool) {
    for (int i = 0; i < static_cast<int>(foodPool.active.size()); ++i) {
        if (foodPool.active[i]) {
            DrawCircleV(foodPool.transform[i].position, foodPool.radius[i], colors::GetNeonColor(colors::NEON_CORAL));
        }
    }
}

void Renderer::RenderRoots(const RootPool& rootPool) {
    for (int i = 0; i < static_cast<int>(rootPool.active.size()); ++i) {
        if (rootPool.active[i]) {
            DrawCircleV(rootPool.transform[i].position, rootPool.radius[i], colors::GetNeonColor(colors::NEON_PURPLE));
        }
    }
}

void Renderer::RenderWalls() {
    // Note: textures are vertically flipped, fix if necessary
    DrawTexture(
        wallsTexture.texture,
        0, 0,
        WHITE
    );
}



// This does NOT need to run every frame unless the texture's contents change
void Renderer::ConfigWallsTexture(const std::vector<Rectangle>& walls) {
    BeginTextureMode(wallsTexture);

    ClearBackground(BLANK); // invisible background
    for (int i = 0; i < static_cast<int>(walls.size()); ++i) {
        DrawRectangleRec(walls[i], colors::GetNeonColor(colors::NEON_WHITE));
    }

    EndTextureMode();
}
