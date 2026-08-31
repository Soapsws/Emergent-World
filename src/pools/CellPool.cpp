#include <raylib.h>
#include "CellPool.hpp"

CellPool::CellPool(int capacity) {
    state.resize(capacity);
    spawning.resize(capacity);
    radius.resize(capacity);
    dps.resize(capacity);
    drag.resize(capacity);
    visionRadius.resize(capacity);
    active.resize(capacity, false);
}

CellPool::~CellPool() {
    // Cleanup logic if needed
}

int CellPool::CreateNext(const cells::CellData& d) {
    for(int i = 0; i < static_cast<int>(active.size()); ++i) {
        if (!active[i]) { return Create(i, d); }
    }
    return -1; // no free slot
}

int CellPool::Create(int index, const cells::CellData& d) {
    return Create(index, d.state, d.radius, d.dps, d.drag, d.visionRadius, d.spawning.lifetime, d.spawning.cooldown);
}

int CellPool::Create(int index, State stateValue, float rad, float dmg, float dragValue, float visionRadiusValue, float life, float cool) {
    if (index < 0 || index >= static_cast<int>(active.size())) { return -1; }
    // using separate parameter names; otherwise would have to use this-> to disambiguate
    state[index] = stateValue;
    spawning[index] = {life, cool};
    radius[index] = rad;
    dps[index] = dmg;
    drag[index] = dragValue;
    visionRadius[index] = visionRadiusValue;
    active[index] = true;
    return index;
}
