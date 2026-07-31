#pragma once

#include <raylib.h>
#include <vector>
#include "sim_constants.hpp"
#include "IDRegistry.hpp"

class EntityFactory;

// food source (see roots/rhizosphere)

struct RootPool {

    static constexpr world::EntityType entityType = world::EntityType::Root;

    std::vector<entity::Transform> transform;
    std::vector<entity::Spawning> spawning;
    std::vector<float> radius;
    std::vector<int> spawnRate;
    std::vector<float> spawnCooldown;
    std::vector<bool> active;

    RootPool(int capacity = roots::MAX_ROOTS);
    ~RootPool();

    int Create(int index, Vector2 pos, Vector2 vel, float rad, int rate, float life, float cool);
    int Create(int index, const roots::RootData& d);
    int CreateNext(const roots::RootData& d);

    void SpawnFood(int index, EntityFactory& entityFactory, IDRegistry& registry, float dt);
};
