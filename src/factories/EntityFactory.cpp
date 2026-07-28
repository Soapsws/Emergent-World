#include "EntityFactory.hpp"

EntityFactory::EntityFactory(CellPool& cellPool, FoodPool& foodPool, RootPool& rootPool)
    : cellPool(cellPool), foodPool(foodPool), rootPool(rootPool) {}


// Agentic Entities

int EntityFactory::CreateCell(Vector2 pos, Vector2 vel, float rad, float hp, float dps,
                              float life, float cool) {
    return cellPool.CreateNext({
        world::EntitySpecies::Cell,
        entity::Transform{pos, vel},
        entity::Spawning{life, cool},
        rad, hp, dps, true
    });
}
int EntityFactory::CreateCell(const cells::CellData& data) {
    return cellPool.CreateNext(data);
}
void EntityFactory::RespawnWithData(int index, const cells::CellData& data) {
    cellPool.Create(index, data);
}

int EntityFactory::CreateFood(Vector2 pos, Vector2 vel, float rad, float hp,
                              float life, float cool) {
    return foodPool.CreateNext({
        world::FoodTypes::Food_All,
        entity::Transform{pos, vel},
        entity::Spawning{life, cool},
        rad, hp, true
    });
}
int EntityFactory::CreateFood(const food::FoodData& data) {
    return foodPool.CreateNext(data);
}
void EntityFactory::RespawnWithData(int index, const food::FoodData& data) {
    foodPool.Create(index, data);
}

// Environment Entities

int EntityFactory::CreateRoot(Vector2 pos, Vector2 vel, float rad, int rate,
                              float life, float cool) {
    return rootPool.CreateNext({
        entity::Transform{pos, vel},
        entity::Spawning{life, cool},
        rad, rate, true
    });
}
int EntityFactory::CreateRoot(const roots::RootData& data) {
    return rootPool.CreateNext(data);
}
void EntityFactory::RespawnWithData(int index, const roots::RootData& data) {
    rootPool.Create(index, data);
}
