#pragma once
#include "FoodPool.hpp"
#include "sim_constants.hpp"

class FoodFactory {
public:
    FoodFactory(FoodPool& pool);
    ~FoodFactory();

    int CreateFood(Vector2 pos, Vector2 vel, float rad, float hp, float life = 0.0f, float cool = 0.0f);
    int CreateFood(const food::FoodData& d);
    void RespawnWithData(int index, const food::FoodData& d);

private:
    FoodPool& foodPool;
};
