#pragma once
#include <vector>
#include <raylib.h>
#include "sim_constants.hpp"

struct CellPool {

    // Cell properties (Structure of Arrays)
    std::vector<Vector2> position;
    std::vector<Vector2> velocity;
    std::vector<float> health;
    std::vector<float> lifetime;
    std::vector<bool> active;

    // initialize pool with capacity (defaults to cells::MAX_CELLS)
    CellPool(int capacity = cells::MAX_CELLS);

    ~CellPool();

    // Create by explicit index (used by Sim.cpp)
    int Create(int idx, Vector2 pos, Vector2 vel, float hp, float life = 10.0f);

    // Create from CellData (AoS -> SoA unpack)
    int Create(int idx, const cells::CellData& d);

    // Find first inactive slot and create there
    int CreateNext(const cells::CellData& d);
};