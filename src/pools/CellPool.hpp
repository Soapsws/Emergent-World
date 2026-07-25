#pragma once
#include <vector>
#include <raylib.h>
#include "sim_constants.hpp"

struct CellPool {

    // Cell properties (Structure of Arrays)
    std::vector<entity::Transform> transform;
    std::vector<entity::Spawning> spawning;
    std::vector<float> radius;
    std::vector<float> health;
    std::vector<float> dps;
    std::vector<bool> active;

    // initialize pool with capacity (defaults to cells::MAX_CELLS)
    CellPool(int capacity = cells::MAX_CELLS);

    ~CellPool();

    // Create by explicit index (used by Sim.cpp)
    int Create(int idx, Vector2 pos, Vector2 vel, float rad, float hp, float dps, float life, float cool);

    // Create from CellData (AoS -> SoA unpack)
    int Create(int idx, const cells::CellData& d);

    // Find first inactive slot and create there
    int CreateNext(const cells::CellData& d);
};