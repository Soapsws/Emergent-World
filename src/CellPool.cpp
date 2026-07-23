#include <raylib.h>
#include "CellPool.hpp"

CellPool::CellPool(int capacity) {
    transform.resize(capacity);
    spawning.resize(capacity);
    radius.resize(capacity);
    health.resize(capacity);
    dps.resize(capacity);
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
    return Create(index, d.transform.position, d.transform.velocity, d.radius, d.health, d.dps, d.spawning.lifetime, d.spawning.cooldown);
}

int CellPool::Create(int index, Vector2 pos, Vector2 vel, float rad, float hp, float dmg, float life, float cool) {
    if (index < 0 || index >= static_cast<int>(active.size())) { return -1; }
    // using separate parameter names; otherwise would have to use this-> to disambiguate
    transform[index] = {pos, vel};
    spawning[index] = {life, cool};
    radius[index] = rad;
    health[index] = hp;
    dps[index] = dmg;
    active[index] = true;
    return index;
}