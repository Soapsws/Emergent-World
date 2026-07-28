#pragma once

#include "CellPool.hpp"
#include "FoodPool.hpp"
#include "RootPool.hpp"

class EntityFactory {
public:
    EntityFactory(CellPool& cellPool, FoodPool& foodPool, RootPool& rootPool);

    int CreateCell(Vector2 pos, Vector2 vel, float rad, float hp, float dps, float life, float cool);
    int CreateCell(const cells::CellData& data);
    void RespawnWithData(int index, const cells::CellData& data);

    int CreateFood(Vector2 pos, Vector2 vel, float rad, float hp, float life, float cool);
    int CreateFood(const food::FoodData& data);
    void RespawnWithData(int index, const food::FoodData& data);

    int CreateRoot(Vector2 pos, Vector2 vel, float rad, int rate, float life, float cool);
    int CreateRoot(const roots::RootData& data);
    void RespawnWithData(int index, const roots::RootData& data);

private:
    CellPool& cellPool;
    FoodPool& foodPool;
    RootPool& rootPool;
};
