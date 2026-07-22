#include <raylib.h>

#include "Sim.hpp"
#include "CellFactory.hpp"
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



Sim::Sim() : numCells(0), cellPool(cells::MAX_CELLS), cellFactory(cellPool) {
    // Initialize the cell pool with some cells
    for (int i = 0; i < cells::MAX_CELLS; ++i) {
        cells::CellData data = sim::defaultSpawn();
        int id = cellFactory.CreateCell(data);
        if (id >= 0) ++numCells; // increments then yields new value
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
    // Update logic for cells can be added here
    for (int i = 0; i < numCells; ++i) {
        if (cellPool.active[i]) {
            // Update position based on velocity
            cellPool.position[i].x += cellPool.velocity[i].x;
            cellPool.position[i].y += cellPool.velocity[i].y;

            if (cellPool.position[i].x < 0 || cellPool.position[i].x > settings::SCREEN_WIDTH) {
                cellPool.velocity[i].x *= -1; // Reverse direction
            }
            if (cellPool.position[i].y < 0 || cellPool.position[i].y > settings::SCREEN_HEIGHT) {
                cellPool.velocity[i].y *= -1; // Reverse direction
            }

            cellPool.lifetime[i] -= GetFrameTime();
            if (cellPool.lifetime[i] <= 0) {
                cellPool.active[i] = false;
            }
        } else {
            // Respawn the cell
            cellPool.position[i] = { static_cast<float>(GetRandomValue(0, settings::SCREEN_WIDTH)), static_cast<float>(GetRandomValue(0, settings::SCREEN_HEIGHT)) };
            cellPool.velocity[i] = { static_cast<float>(GetRandomValue(-1, 1)), static_cast<float>(GetRandomValue(-1, 1)) };
            cellPool.health[i] = static_cast<float>(GetRandomValue(50, 100));
            cellPool.lifetime[i] = 100.0f;
            cellPool.active[i] = true;
        }
    }
}

void Sim::Render() {
    BeginDrawing();
    ClearBackground(BLACK);

    // Render cells
    for (int i = 0; i < numCells; ++i) {
        if (cellPool.active[i]) {
            DrawCircleV(cellPool.position[i], cells::CELL_RADIUS, GREEN);
        }
    }

    EndDrawing();
}
