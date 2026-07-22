#include <raylib.h>
#include "CellFactory.hpp"


CellFactory::CellFactory(CellPool& pool) : cellPool(pool) {}

CellFactory::~CellFactory() {
    // Cleanup logic if needed
}

int CellFactory::CreateCell(Vector2 pos, Vector2 vel, float hp, float life) {
    cells::CellData d{pos, vel, hp, life, true};
    return cellPool.CreateNext(d);
}

int CellFactory::CreateCell(const cells::CellData& d) {
    return cellPool.CreateNext(d);
}
