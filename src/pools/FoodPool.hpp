#pragma once
#include <vector>
#include <raylib.h>
#include "sim_constants.hpp"

struct FoodPool {

    static constexpr world::EntityType entityType = world::EntityType::Food;

    std::vector<entity::Transform> transform;
    std::vector<entity::Spawning> spawning;
    std::vector<float> radius;
    std::vector<float> health;
    std::vector<float> drag;
    std::vector<bool> active;

    FoodPool(int capacity = food::MAX_NATURAL_FOOD);
    ~FoodPool();


    // rewired to spawn at Root Pools
    int Create(int idx, Vector2 pos, Vector2 vel, float rad, float hp, float drag, float life, float cool);
    int Create(int idx, const food::FoodData& d);
    int CreateNext(const food::FoodData& d);
};
