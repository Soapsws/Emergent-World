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



Sim::Sim() : numCells(0), numFood(0), cellPool(cells::MAX_CELLS), cellFactory(cellPool), 
                foodPool(food::MAX_NATURAL_FOOD), foodFactory(foodPool), renderer(), walls() {
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

    renderer.ManualInit();
    ManualTestWalls();
    renderer.ConfigWallsTexture(walls.walls);
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

    // USER INPUT

    ProcessInput();
}

void Sim::ProcessInput() {
    // (self-reminder: move to bottom)
    if (IsKeyDown(KEY_UP)) renderer.pcam.pan(270); // inverted y axis
    if (IsKeyDown(KEY_DOWN)) renderer.pcam.pan(90);
    if (IsKeyDown(KEY_LEFT)) renderer.pcam.pan(180);
    if (IsKeyDown(KEY_RIGHT)) renderer.pcam.pan(0);

    if (IsKeyDown(KEY_I)) renderer.pcam.zoom(1.0 + renderer.pcam.zoomScale * GetFrameTime()); // so it's capped regardless of device specs
    if (IsKeyDown(KEY_O)) renderer.pcam.zoom(1/(1.0 + renderer.pcam.zoomScale * GetFrameTime()));

    // implement following later
}

template <typename Pool>
void Sim::UpdateMovement(Pool& pool, int numEntities) {
    for (int i = 0; i < numEntities; ++i) {
        if (pool.active[i]) {
            pool.transform[i].position.x += pool.transform[i].velocity.x;
            pool.transform[i].position.y += pool.transform[i].velocity.y;
            if (pool.transform[i].position.x < 0) {
                pool.transform[i].position.x = settings::WORLD_WIDTH;
            } else if (pool.transform[i].position.x > settings::WORLD_WIDTH) {
                pool.transform[i].position.x = 0;
            }
            if (pool.transform[i].position.y < 0) {
                pool.transform[i].position.y = settings::WORLD_HEIGHT;
            } else if (pool.transform[i].position.y > settings::WORLD_HEIGHT) {
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
            if (pool.health[i] <= 0) {
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

    auto cellFoodInteractor = [&](auto& poolA,  int indexA, auto& poolB, int indexB) {
        interactors.CellFood(poolA, indexA, poolB, indexB);
    };

    auto foodCellInteractor = [&](auto& poolA,  int indexA, auto& poolB, int indexB) {
        ;
        // all handled in cell-food (?)
    };


    auto foodFoodInteractor = [&](auto& poolA, int indexA, auto& poolB, int indexB) {
        interactors.FoodFood(poolA, indexA, poolB, indexB);
    };

    CircleCircleCollision(cellPool, cellCellInteractor, cellPool, cellCellInteractor);
    CircleCircleCollision(cellPool, cellFoodInteractor, foodPool, foodCellInteractor);
    CircleCircleCollision(foodPool, foodFoodInteractor, foodPool, foodFoodInteractor);
}

template <typename CircularEntityPool1, typename Interact1, typename CircularEntityPool2, typename Interact2>
void Sim::CircleCircleCollision(CircularEntityPool1& pool1, Interact1 interactor1, CircularEntityPool2& pool2, Interact2 interactor2) {
    // If both pools are actually the same object, avoid duplicate checks and self-collision
    bool samePool =
        ( static_cast<const void*>(std::addressof(pool1)) ==
        static_cast<const void*>(std::addressof(pool2)) );

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
    //------------------
    BeginDrawing(); // DRAWING
    //------------------
        BeginMode2D(renderer.pcam.data()); // CAMERA2D
    //------------------_-_-_-_-_-_-_

    ClearBackground(BLACK);

    renderer.RenderCells(cellPool);
    renderer.RenderFood(foodPool);
    
    renderer.RenderWalls(); // NEW

    //------------------_-_-_-_-_-_-_
        EndMode2D();  // CAMERA2D
    //------------------
    EndDrawing(); // DRAWING
    //------------------
}

// TEMPORARY MANUAL TEST: remove this function and its constructor call when real walls are ready.
void Sim::ManualTestWalls() {
    constexpr float wallThickness = 50.0f;
    const float worldWidth = static_cast<float>(settings::WORLD_WIDTH);
    const float worldHeight = static_cast<float>(settings::WORLD_HEIGHT);

    walls.AddWall(0.0f, 0.0f, worldWidth, wallThickness); // top
    walls.AddWall(0.0f, worldHeight - wallThickness, worldWidth, wallThickness); // bottom
    walls.AddWall(0.0f, wallThickness, wallThickness, worldHeight - 2.0f * wallThickness); // left
    walls.AddWall(worldWidth - wallThickness, wallThickness, wallThickness, worldHeight - 2.0f * wallThickness); // right
}

