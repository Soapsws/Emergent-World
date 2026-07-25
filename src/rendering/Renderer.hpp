#pragma once

#include "Camera.hpp"
#include "CellPool.hpp"
#include "FoodPool.hpp"

struct Renderer {
    PlayerCamera pcam;

    Renderer();
    ~Renderer() = default;

    void RenderCells(const CellPool& cellPool);
    void RenderFood(const FoodPool& foodPool);

    

};
