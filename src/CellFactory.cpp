#include <raylib.h>
#include "CellFactory.hpp"


CellFactory::CellFactory(CellPool& pool) : cellPool(pool) {}

CellFactory::~CellFactory() {
    // Cleanup logic if needed
}

int CellFactory::CreateCell(Vector2 pos, Vector2 vel, float rad, float hp, float life, float cool) {
    cells::CellData d{
        world::EntitySpecies::Cell,
        entity::Transform{pos, vel},
        entity::Spawning{life, cool},
        rad,
        hp,
        0.0f,
        true
    };
    return cellPool.CreateNext(d);
}

int CellFactory::CreateCell(const cells::CellData& d) {
    return cellPool.CreateNext(d);
}

void CellFactory::RespawnWithData(int index, const cells::CellData& d) {
   cellPool.transform[index] = d.transform;
   cellPool.spawning[index] = d.spawning;
   cellPool.radius[index] = d.radius;
   cellPool.health[index] = d.health;
   cellPool.active[index] = true;
}
