#pragma once
#include <vector>
#include <raylib.h>
#include "sim_constants.hpp"

struct FoodPool {
    std::vector<entity::Transform> transform;
    std::vector<entity::Spawning> spawning;
    std::vector<float> radius;
    std::vector<float> health;
    std::vector<bool> active;

    FoodPool(int capacity = food::MAX_NATURAL_FOOD);
    ~FoodPool();

    int Create(int idx, Vector2 pos, Vector2 vel, float rad, float hp, float life, float cool);
    int Create(int idx, const food::FoodData& d);
    int CreateNext(const food::FoodData& d);
};
