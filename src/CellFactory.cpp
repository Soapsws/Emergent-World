#include <raylib.h>
#include "CellFactory.hpp"


CellFactory::CellFactory(CellPool& pool) : cellPool(pool) {}

CellFactory::~CellFactory() {
    // Cleanup logic if needed
}

int CellFactory::CreateCell(Vector2 pos, Vector2 vel, float rad, float hp, float life, float cool) {
    cells::CellData d{pos, vel, rad, hp, life, cool, true};
    return cellPool.CreateNext(d);
}

int CellFactory::CreateCell(const cells::CellData& d) {
    return cellPool.CreateNext(d);
}

void CellFactory::RespawnWithData(int index, const cells::CellData& d) {
   cellPool.position[index] = d.position;
   cellPool.velocity[index] = d.velocity;
   cellPool.radius[index] = d.radius;
   cellPool.health[index] = d.health;
   cellPool.lifetime[index] = d.lifetime;
   cellPool.cooldown[index] = d.cooldown;
   cellPool.active[index] = true;
}
