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
        cells::CellData data = sim::defaultSpawn();
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
    for (int i = 0; i < numCells; ++i) {
        if (cellPool.active[i]) {
            cellPool.transform[i].position.x += cellPool.transform[i].velocity.x;
            cellPool.transform[i].position.y += cellPool.transform[i].velocity.y;

            if (cellPool.transform[i].position.x < 0 || cellPool.transform[i].position.x > settings::SCREEN_WIDTH) {
                cellPool.transform[i].velocity.x *= -1;
            }
            if (cellPool.transform[i].position.y < 0 || cellPool.transform[i].position.y > settings::SCREEN_HEIGHT) {
                cellPool.transform[i].velocity.y *= -1;
            }

            cellPool.spawning[i].lifetime -= GetFrameTime();
            if (cellPool.spawning[i].lifetime <= 0) {
                cellPool.active[i] = false;
            }
        } else {
            cellPool.spawning[i].cooldown -= GetFrameTime();
            if (cellPool.spawning[i].cooldown <= 0) {
                cells::CellData data = sim::defaultSpawn();
                cellFactory.RespawnWithData(i, data);
            }
        }
    }

    for (int i = 0; i < numFood; ++i) {
        if (foodPool.active[i]) {
            foodPool.transform[i].position.x += foodPool.transform[i].velocity.x;
            foodPool.transform[i].position.y += foodPool.transform[i].velocity.y;

            if (foodPool.transform[i].position.x < 0) {
                foodPool.transform[i].position.x = settings::SCREEN_WIDTH;
            } else if (foodPool.transform[i].position.x > settings::SCREEN_WIDTH) {
                foodPool.transform[i].position.x = 0;
            }
            if (foodPool.transform[i].position.y < 0) {
                foodPool.transform[i].position.y = settings::SCREEN_HEIGHT;
            } else if (foodPool.transform[i].position.y > settings::SCREEN_HEIGHT) {
                foodPool.transform[i].position.y = 0;
            }

            foodPool.spawning[i].lifetime -= GetFrameTime();
            if (foodPool.spawning[i].lifetime <= 0) {
                foodPool.active[i] = false;
            }
        } else {
            foodPool.spawning[i].cooldown -= GetFrameTime();
            if (foodPool.spawning[i].cooldown <= 0) {
                food::FoodData data = food::defaultSpawn();
                foodFactory.RespawnWithData(i, data);
            }
        }
    }
}

void Sim::Render() {
    BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 0; i < numCells; ++i) {
        if (cellPool.active[i]) {
            DrawCircleV(cellPool.position[i], cellPool.radius[i], GREEN);
        }
    }

    for (int i = 0; i < numFood; ++i) {
        if (foodPool.active[i]) {
            DrawCircleV(foodPool.position[i], foodPool.radius[i], RED);
        }
    }

    EndDrawing();
}
