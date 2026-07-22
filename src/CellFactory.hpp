#pragma once
#include "CellPool.hpp"
#include "sim_constants.hpp"


/*

Currently, this is redundant due to simply echoing functions to CellPool.
However, once more entity types are made, this can be refactored into a general EntityFactory 
    to provide idiomatic functionality.

*/


class CellFactory {
public:
    CellFactory(CellPool& pool);
    ~CellFactory();

    // Create from explicit fields, will try to find a free slot
    int CreateCell(Vector2 pos, Vector2 vel, float hp, float life = 100.0f);

    // Create from an existing CellData (unpack AoS -> SoA)
    int CreateCell(const cells::CellData& d);

private:
    CellPool& cellPool;
};