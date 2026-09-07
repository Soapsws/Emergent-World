#include "FoodPool.hpp"

FoodPool::FoodPool(int capacity) {
    transform.resize(capacity);
    spawning.resize(capacity);
    radius.resize(capacity);
    health.resize(capacity);
    drag.resize(capacity);
    consumable.resize(capacity);
    active.resize(capacity, false);
}

FoodPool::~FoodPool() {
    // Cleanup logic if needed
}

int FoodPool::CreateNext(const food::FoodData& d) {
    for (int i = 0; i < static_cast<int>(active.size()); ++i) {
        if (!active[i]) {
            return Create(i, d);
        }
    }
    return -1;
}

int FoodPool::Create(int index, const food::FoodData& d) {
    return Create(index, d.transform.position, d.transform.velocity, d.radius, d.health, d.drag, d.spawning.lifetime, d.spawning.cooldown, d.consumable);
}

int FoodPool::Create(int index, Vector2 pos, Vector2 vel, float rad, float hp, float dragValue, float life, float cool, entity::Consumable consumableValues) {
    if (index < 0 || index >= static_cast<int>(active.size())) {
        return -1;
    }

    transform[index] = {pos, vel};
    spawning[index] = {life, cool};
    radius[index] = rad;
    health[index] = hp;
    drag[index] = dragValue;
    consumable[index] = consumableValues;
    active[index] = true;
    return index;
}
