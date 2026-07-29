#include "RootPool.hpp"
#include "EntityFactory.hpp"
#include <cmath>
#include <raymath.h>

RootPool::RootPool(int capacity) {
    transform.resize(capacity);
    spawning.resize(capacity);
    radius.resize(capacity);
    spawnRate.resize(capacity);
    spawnCooldown.resize(capacity);
    active.resize(capacity, false);
}

RootPool::~RootPool() {
    // Cleanup logic if needed
}

int RootPool::CreateNext(const roots::RootData& d) {
    for (int i = 0; i < static_cast<int>(active.size()); ++i) {
        if (!active[i]) return Create(i, d);
    }
    return -1;
}

int RootPool::Create(int index, const roots::RootData& d) {
    return Create(index, d.transform.position, d.transform.velocity, d.radius,
                  d.spawnRate, d.spawning.lifetime, d.spawning.cooldown);
}

int RootPool::Create(int index, Vector2 pos, Vector2 vel, float rad, int rate,
                     float life, float cool) {
    if (index < 0 || index >= static_cast<int>(active.size())) return -1;

    transform[index] = {pos, vel};
    spawning[index] = {life, cool};
    radius[index] = rad;
    spawnRate[index] = rate;
    spawnCooldown[index] = static_cast<float>(rate);
    active[index] = true;
    return index;
}

void RootPool::SpawnFood(int index, EntityFactory& entityFactory, float dt) {
    if (index < 0 || index >= static_cast<int>(active.size()) || !active[index]) return;

    spawnCooldown[index] -= dt;
    if (spawnCooldown[index] <= 0.0f) {
        const food::FoodData defaults = food::defaultSpawn();
        const float angle = math::GetRandomFloat(0.0f, 2.0f * PI);
        const Vector2 direction{ cosf(angle), sinf(angle) };
        const float gap = 1.0f;
        const float distance = radius[index] + defaults.radius + gap;
        const Vector2 position = Vector2Add(
            transform[index].position,
            Vector2Scale(direction, distance)
        );
        const Vector2 velocity = Vector2Scale(direction, 1.0f);

        const int foodIndex = entityFactory.CreateFood(
            position,
            velocity,
            defaults.radius,
            defaults.health,
            defaults.drag,
            defaults.spawning.lifetime,
            defaults.spawning.cooldown
        );

        if (foodIndex >= 0) {
            spawnCooldown[index] = static_cast<float>(spawnRate[index]);
        }
    }
}
