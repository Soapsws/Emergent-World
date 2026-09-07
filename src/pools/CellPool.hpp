#pragma once
#include <vector>
#include <raylib.h>
#include "sim_constants.hpp"
#include "State.hpp"

struct CellPool {

    static constexpr world::EntityType entityType = world::EntityType::Cell;

    // Cell properties (Structure of Arrays)
    std::vector<State> state;
    std::vector<entity::Spawning> spawning;
    std::vector<float> radius;
    std::vector<float> dps;
    std::vector<float> drag;
    std::vector<float> visionRadius;
    std::vector<float> raycastLength;
    std::vector<entity::Vitals> vitals;
    std::vector<entity::Consumable> consumable;
    std::vector<bool> active;

    // initialize pool with capacity (defaults to cells::MAX_CELLS)
    CellPool(int capacity = cells::MAX_CELLS);

    ~CellPool();

    // Create by explicit index (used by Sim.cpp)
    int Create(int idx, State state, float rad, float dps, float drag, float visionRadius, float raycastLength, 
        float life, float cool,  entity::Vitals vitals, entity::Consumable consumable);

    // Create from CellData (AoS -> SoA unpack)
    int Create(int idx, const cells::CellData& d);

    // Find first inactive slot and create there
    int CreateNext(const cells::CellData& d);
};
