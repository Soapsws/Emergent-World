#include "FoodFactory.hpp"

FoodFactory::FoodFactory(FoodPool& pool) : foodPool(pool) {}

FoodFactory::~FoodFactory() {
    // Cleanup logic if needed
}

int FoodFactory::CreateFood(Vector2 pos, Vector2 vel, float rad, float hp, float life, float cool) {
    food::FoodData d{
        world::FoodTypes::Food_All,
        entity::Transform{pos, vel},
        entity::Spawning{life, cool},
        rad,
        hp,
        true
    };
    return foodPool.CreateNext(d);
}

int FoodFactory::CreateFood(const food::FoodData& d) {
    return foodPool.CreateNext(d);
}

void FoodFactory::RespawnWithData(int index, const food::FoodData& d) {
    foodPool.transform[index] = d.transform;
    foodPool.spawning[index] = d.spawning;
    foodPool.radius[index] = d.radius;
    foodPool.health[index] = d.health;
    foodPool.active[index] = true;
}
