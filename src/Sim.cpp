#include <raylib.h>

#include "Sim.hpp"
#include "sim_constants.hpp"


/*
-- Constructor Intuition --
Object::Object(<onstructor parameter list>) : <member initialization list> {
    constructor body
}

The constructor parameter list defines what callers pass in.
The member initialization list initializes class members before the constructor body runs.
    Java member initialization is done in the body; in C++ the initializer list is more efficient 
    and allows for const members to be initialized.
    * Essentially the same idea, C++ just prefers this.
    
    There are different cases: for primitive types, you pass in the intended value in parentheses. 
        for references you pass in the object to bind to. for objects you pass in the constructor parameters for that object.


    Parameters passed in in the constructor parameter list can be used in the member initialization list
    and also inside the constructor body.
*/



Sim::Sim() : numCells(0), numFood(0), cellPool(cells::MAX_CELLS), cellFactory(cellPool), foodPool(food::MAX_NATURAL_FOOD), foodFactory(foodPool) {
    for (int i = 0; i < cells::MAX_CELLS; ++i) {
        cells::CellData data = cells::defaultSpawn();
        int id = cellFactory.CreateCell(data);
        if (id >= 0) ++numCells;
    }

    for (int i = 0; i < food::MAX_NATURAL_FOOD; ++i) {
        food::FoodData data = food::defaultSpawn();
        int id = foodFactory.CreateFood(data);
        if (id >= 0) ++numFood;
    }

    InitWindow(settings::SCREEN_WIDTH, settings::SCREEN_HEIGHT, "Emergent World");
    SetTargetFPS(60);
}

Sim::~Sim() {
    CloseWindow();
}

void Sim::Run() {
    while (!WindowShouldClose()) {
        Update();
        Render();
    }
}

void Sim::Update() {
    UpdateMovement(cellPool, numCells);
    // C++ lambda: [what to capture] { body }
    // Below is a callable that takes no arguments and returns a new entity spawn data
            // Full form: [ capture_clause ] ( parameter_list ) specifiers -> return_type { body }
            // "-> return type" can be omitted unless it must be specified.
    // If you just pass the function you'd have to call it INSIDE the function and manually capture the function.
    UpdateSpawning(cellPool, cellFactory, [] { return cells::defaultSpawn(); }, numCells);

    UpdateMovement(foodPool, numFood);
    UpdateSpawning(foodPool, foodFactory, [] { return food::defaultSpawn(); }, numFood);
}

template <typename Pool>
void Sim::UpdateMovement(Pool& pool, int numEntities) {
    for (int i = 0; i < numEntities; ++i) {
        if (pool.active[i]) {
            pool.transform[i].position.x += pool.transform[i].velocity.x;
            pool.transform[i].position.y += pool.transform[i].velocity.y;
        }
    }
}

template <typename Pool, typename Factory, typename DefaultSpawn>
void Sim::UpdateSpawning(Pool& pool, Factory& factory, DefaultSpawn defaultSpawn, int numEntities) {
    float t = GetFrameTime();
    for (int i = 0; i < numEntities; ++i) {
        if (pool.active[i]) {
            pool.spawning[i].lifetime -= t;
            if (pool.spawning[i].lifetime <= 0) {
                pool.active[i] = false;
            }
        } else {
            pool.spawning[i].cooldown -= t;
            if (pool.spawning[i].cooldown <= 0) {
                // auto deduces data from the RHS
                auto data = defaultSpawn();
                factory.RespawnWithData(i, data);
            }
        }
    }
}

void Sim::CellCellCollision() {

}


void Sim::Render() {
    BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 0; i < numCells; ++i) {
        if (cellPool.active[i]) {
            DrawCircleV(cellPool.transform[i].position, cellPool.radius[i], GREEN);
        }
    }

    for (int i = 0; i < numFood; ++i) {
        if (foodPool.active[i]) {
            DrawCircleV(foodPool.transform[i].position, foodPool.radius[i], RED);
        }
    }

    EndDrawing();
}
