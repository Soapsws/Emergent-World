#include <raylib.h>
#include <optional>
#include <type_traits>

#include <memory>
#include <cmath>

#include <algorithm>
#include <numeric>
#include <random>

#include "Sim.hpp"
#include "sim_constants.hpp"
#include "Interactors.hpp"
#include "Physics.hpp"


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



Sim::Sim() : numCells(0), numFood(0), numRoots(0), 
                cellPool(cells::MAX_CELLS), foodPool(food::MAX_NATURAL_FOOD), rootPool(roots::MAX_ROOTS),
                entityFactory(cellPool, foodPool, rootPool),
                maxCells(cells::MAX_CELLS), maxFood(food::MAX_NATURAL_FOOD), maxRoots(roots::MAX_ROOTS),
                walls(),
                renderer(),
                gui(true, renderer.pcam, maxCells, maxFood) {

    // Initial spawn (maximum counts)

    for (int i = 0; i < maxCells; ++i) {
        cells::CellData data = cells::defaultSpawn();
        int id = entityFactory.CreateCell(data);
        if (id >= 0) ++numCells;
    }
    for (int i = 0; i < maxRoots; ++i) {
        int id = entityFactory.CreateRoot(roots::defaultSpawn());
        if (id >= 0) ++numRoots;
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
    // C++ lambda: [what to capture] { body }
    // Below is a callable that takes no arguments and returns a new entity spawn data
            // Full form: [ capture_clause ] ( parameter_list ) specifiers -> return_type { body }
            // "-> return type" can be omitted unless it must be specified.
    // If you just pass the function you'd have to call it INSIDE the function and manually capture the function.
    
    UpdateSpawning(cellPool, entityFactory, [] { return cells::defaultSpawn(); }, maxCells);
    UpdateSpawning(foodPool, entityFactory, [] { return food::defaultSpawn(); }, maxFood, false);
    UpdateSpawning(rootPool, entityFactory, [] { return roots::defaultSpawn(); }, maxRoots);

    // With more unique entity functionality, move this into helper function
    std::vector<int> rootOrder(maxRoots);
    std::iota(rootOrder.begin(), rootOrder.end(), 0); // fills range with sequentially increasing values
    static std::mt19937 rng(std::random_device{}()); // mersenne twister seeded using std::random_device (a rng)
    std::shuffle(rootOrder.begin(), rootOrder.end(), rng); // to allow equal spawn distribution
    for (int i : rootOrder) rootPool.SpawnFood(i, entityFactory, GetFrameTime());

    // Movement
    UpdateMovement(cellPool, maxCells);
    UpdateMovement(foodPool, maxFood);

    UpdateCollisions();

    UpdateEntityHealth(cellPool, maxCells);
    UpdateEntityHealth(foodPool, maxFood);

    // USER INPUT

    ProcessInput();
}

template <typename Pool>
void Sim::UpdateMovement(Pool& pool, int numEntities) {
    const float dt = GetFrameTime();
    for (int i = 0; i < numEntities; ++i) {
        if (pool.active[i]) {
            // Drag is normalized: 0 = no drag. The exponent keeps the result
            // consistent when frame rate changes (drag is tuned at 60 FPS).
            const float retention = std::clamp(1.0f - pool.drag[i], 0.0f, 1.0f);
            const float frameRetention = std::pow(retention, dt * 60.0f);
            pool.transform[i].velocity = Vector2Scale(pool.transform[i].velocity, frameRetention);
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
void Sim::UpdateSpawning(Pool& pool, Factory& factory, DefaultSpawn defaultSpawn,
                         int numEntities, bool respawnInactive) {
    float t = GetFrameTime();

    // handles dynamic pool size adjustments
    for (int i = numEntities; i < static_cast<int>(pool.active.size()); ++i) {
        pool.active[i] = false;
    }

    for (int i = 0; i < numEntities; ++i) {
        if (pool.active[i]) {
            pool.spawning[i].lifetime -= t;
            if (pool.spawning[i].lifetime <= 0) {
                pool.active[i] = false;
            }
        } else if (respawnInactive) {
            pool.spawning[i].cooldown -= t;
            if (pool.spawning[i].cooldown <= 0) {
                // auto deduces data from the RHS
                auto data = defaultSpawn();
                factory.RespawnWithData(i, data);
            }
        }
    }
}

template <typename Pool>
void Sim::UpdateEntityHealth(Pool& pool, int numEntities) {
    for (int i = 0; i < numEntities; ++i) {
        if (pool.active[i] && pool.health[i] <= 0.0f) {
            pool.active[i] = false;
        }
    }
}


void Sim::UpdateCollisions() {

    // final argument: restitution (bounciness of collision, if applicable)

    auto cellCellInteractor = [&](auto& poolA, int indexA, auto& poolB, int indexB) {
        physics::CircularBounce(poolA, indexA, poolB, indexB, 0.8f);
    };

    auto cellFoodInteractor = [&](auto& poolA,  int indexA, auto& poolB, int indexB) {
        interactions::CellFood(poolA, indexA, poolB, indexB);
    };

    auto foodFoodInteractor = [&](auto& poolA, int indexA, auto& poolB, int indexB) {
        physics::CircularBounce(poolA, indexA, poolB, indexB, 0.6f);
    };

    auto cellWallInteractor = [&](auto& poolA, int indexA, auto& poolB, int indexB) {
        physics::CircularImmovableRectangularBounce(poolA, indexA, poolB, indexB, 0.6f);
    };

    auto foodWallInteractor = [&](auto& poolA, int indexA, auto& poolB, int indexB) {
        physics::CircularImmovableRectangularBounce(poolA, indexA, poolB, indexB, 0.6f);
    };

    auto cellRootInteractor = [&](auto& poolA, int indexA, auto& poolB, int indexB) {
        physics::CircularImmovableCircularBounce(poolA, indexA, poolB, indexB, 0.6f);
    };

    auto foodRootInteractor = [&](auto& poolA, int indexA, auto& poolB, int indexB) {
        physics::CircularImmovableCircularBounce(poolA, indexA, poolB, indexB, 0.99f);
    };

    CEntityCEntityCollision(cellPool, cellCellInteractor, cellPool, std::nullopt);
    CEntityCEntityCollision(cellPool, cellFoodInteractor, foodPool, std::nullopt);
    CEntityCEntityCollision(foodPool, foodFoodInteractor, foodPool, std::nullopt);

    CEntityRObjectCollision(cellPool, cellWallInteractor, walls, std::nullopt);
    CEntityRObjectCollision(foodPool, foodWallInteractor, walls, std::nullopt);

}

// Circular Entity - Circular Entity Collision. name shortened to save space
template <typename CircularEntityPool1, typename Interact1, typename CircularEntityPool2, typename Interact2>
void Sim::CEntityCEntityCollision(CircularEntityPool1& pool1, Interact1 interactor1, CircularEntityPool2& pool2, Interact2 interactor2) {
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
                // constexpr specifies it can be evaluated at compile-time. regular "if" is runtime and causes a bad nullopt compilation
                if constexpr (!std::is_same_v<Interact1, std::nullopt_t>) interactor1(pool1, i, pool2, j);
                if constexpr (!std::is_same_v<Interact2, std::nullopt_t>) interactor2(pool2, j, pool1, i);
            }
        }
    }
}

// Circular Entity - Rectangular Object Collision
template <typename CircularEntityPool, typename Interact1, typename RectangularObjectPool, typename Interact2>
void Sim::CEntityRObjectCollision(CircularEntityPool& pool1, Interact1 interactor1, RectangularObjectPool& pool2, Interact2 interactor2) {
    for (int i = 0; i < static_cast<int>(pool1.active.size()); ++i) {
        if (!pool1.active[i]) continue;
        for (int j = 0; j < static_cast<int>(pool2.walls.size()); ++j) {
            if (CheckCollisionCircleRec(
                    pool1.transform[i].position,
                    pool1.radius[i],
                    pool2.walls[j])) {
                if constexpr (!std::is_same_v<Interact1, std::nullopt_t>) interactor1(pool1, i, pool2, j);
                if constexpr (!std::is_same_v<Interact2, std::nullopt_t>) interactor2(pool2, j, pool1, i);
            }
        }
    }
}

void Sim::ProcessInput() {
    if (IsKeyDown(KEY_UP)) renderer.pcam.pan(270); // inverted y axis
    if (IsKeyDown(KEY_DOWN)) renderer.pcam.pan(90);
    if (IsKeyDown(KEY_LEFT)) renderer.pcam.pan(180);
    if (IsKeyDown(KEY_RIGHT)) renderer.pcam.pan(0);

    if (IsKeyDown(KEY_I)) renderer.pcam.zoom(1.0 + renderer.pcam.zoomScale * GetFrameTime()); 
    if (IsKeyDown(KEY_O)) renderer.pcam.zoom(1/(1.0 + renderer.pcam.zoomScale * GetFrameTime()));

    // implement following later
}



void Sim::Render() {
    BeginDrawing(); // DRAWING

        BeginMode2D(renderer.pcam.data()); // CAMERA2D

            ClearBackground(BLACK);

            renderer.RenderCells(cellPool);
            renderer.RenderFood(foodPool);
            renderer.RenderRoots(rootPool);
    
            renderer.RenderWalls(); // NEW

        EndMode2D();  // CAMERA2D

        // ~~~~~~~~ UI outside of Camera2D block so it tracks with the frame, not the world

        gui.Begin();

            gui.Draw();

        gui.End();

    EndDrawing(); // DRAWING
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
