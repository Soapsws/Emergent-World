#pragma once

#include <raylib.h>

#include "Camera.hpp"
#include "CellPool.hpp"
#include "FoodPool.hpp"
#include "RootPool.hpp"

struct Renderer {
    PlayerCamera pcam;

    RenderTexture2D wallsTexture;

    bool isInitialized { false };

    Renderer();
    ~Renderer();

    void ManualInit(); // to make sure it's loaded after InitWindow.

    void RenderCells(const CellPool& cellPool);
    void RenderFood(const FoodPool& foodPool);
    void RenderRoots(const RootPool& rootPool);
    void RenderWalls();

    void ConfigWallsTexture(const std::vector<Rectangle>& walls);
};
