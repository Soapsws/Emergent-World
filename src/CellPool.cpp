#include <raylib.h>
#include "CellPool.hpp"

CellPool::CellPool(int capacity) {
    position.resize(capacity);
    velocity.resize(capacity);
    health.resize(capacity);
    lifetime.resize(capacity);
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
    return Create(index, d.position, d.velocity, d.health, d.lifetime);
}

int CellPool::Create(int index, Vector2 pos, Vector2 vel, float hp, float life) {
    if (index < 0 || index >= static_cast<int>(active.size())) { return -1; }
    // using separate parameter names; otherwise would have to use this-> to disambiguate
    position[index] = pos;
    velocity[index] = vel;
    health[index] = hp;
    lifetime[index] = life;
    active[index] = true;
    return index;
}