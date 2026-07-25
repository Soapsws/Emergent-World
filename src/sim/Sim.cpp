#include <raylib.h>

#include "Sim.hpp"
#include "sim_constants.hpp"
#include "Interactors.hpp"
#include <memory>


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

    UpdateCollisions();
}

template <typename Pool>
void Sim::UpdateMovement(Pool& pool, int numEntities) {
    for (int i = 0; i < numEntities; ++i) {
        if (pool.active[i]) {
            pool.transform[i].position.x += pool.transform[i].velocity.x;
            pool.transform[i].position.y += pool.transform[i].velocity.y;
            if (pool.transform[i].position.x < 0) {
                pool.transform[i].position.x = settings::SCREEN_WIDTH;
            } else if (pool.transform[i].position.x > settings::SCREEN_WIDTH) {
                pool.transform[i].position.x = 0;
            }
            if (pool.transform[i].position.y < 0) {
                pool.transform[i].position.y = settings::SCREEN_HEIGHT;
            } else if (pool.transform[i].position.y > settings::SCREEN_HEIGHT) {
                pool.transform[i].position.y = 0;
            }
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

void Sim::UpdateCollisions() {
    Interactors interactors;

    // Generic Lambda

    auto cellCellInteractor = [&](auto& poolA, int indexA, auto& poolB, int indexB) {
        interactors.CellCell(poolA, indexA, poolB, indexB);
    };

    CircleCircleCollision(cellPool, cellCellInteractor, cellPool, cellCellInteractor);
    // CircleCircleCollision(cellPool, cellFoodInteractor, foodPool, foodCellInteractor);
    // CircleCircleCollision(foodPool, foodFoodInteractor, foodPool, foodFoodInteractor);
}

template <typename CircularEntityPool1, typename Interact1, typename CircularEntityPool2, typename Interact2>
void Sim::CircleCircleCollision(CircularEntityPool1& pool1, Interact1 interactor1, CircularEntityPool2& pool2, Interact2 interactor2) {
    // If both pools are actually the same object, avoid duplicate checks and self-collision
    bool samePool = (std::addressof(pool1) == std::addressof(pool2));

    for (int i = 0; i < static_cast<int>(pool1.active.size()); ++i) {
        if (!pool1.active[i]) continue;

        int jStart = 0;
        if (samePool) jStart = i + 1;

        for (int j = jStart; j < static_cast<int>(pool2.active.size()); ++j) {
            if (!pool2.active[j]) continue;

            // Collision check
            Vector2 pos1 = pool1.transform[i].position;
            float rad1 = pool1.radius[i];
            Vector2 pos2 = pool2.transform[j].position;
            float rad2 = pool2.radius[j];

            if (CheckCollisionCircles(pos1, rad1, pos2, rad2)) {
                if (samePool) interactor1(pool1, i, pool2, j);
                else {
                    interactor1(pool1, i, pool2, j);
                    interactor2(pool2, j, pool1, i);
                }
            }
        }
    }
}


void Sim::Render() {
    BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 0; i < numCells; ++i) {
        if (cellPool.active[i]) {
            DrawCircleV(cellPool.transform[i].position, cellPool.radius[i], colors::GetNeonColor(colors::NEON_AZURE));
        }
    }

    for (int i = 0; i < numFood; ++i) {
        if (foodPool.active[i]) {
            DrawCircleV(foodPool.transform[i].position, foodPool.radius[i], colors::GetNeonColor(colors::NEON_PINK));
        }
    }

    EndDrawing();
}
