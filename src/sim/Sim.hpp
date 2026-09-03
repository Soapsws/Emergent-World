#pragma once

#include <functional>
#include <optional>
#include <variant>

#include "CellPool.hpp"
#include "FoodPool.hpp"
#include "RootPool.hpp"
#include "EntityFactory.hpp"
#include "IDRegistry.hpp"
#include "HashGrid.hpp"
#include "TransformAccess.hpp"
#include "HashGrid.hpp"
#include "Renderer.hpp"
#include "Walls.hpp"
#include "UI.hpp"
#include "Action.hpp"
#include "Brain.hpp"

class Sim {

    public: 

        // in .hpp the constructor declaration does not include member initializers. 
        Sim();
        ~Sim();

        void Run();

    private:

        // std::variant is type-safe and means SelectedPool can hold a reference to exactly one of these.
        // reference_wrapper means the pool isn't copied; it's just a reference.
        using SelectedPool = std::variant<
            std::reference_wrapper<CellPool>,
            std::reference_wrapper<FoodPool>,
            std::reference_wrapper<RootPool>
        >;

        void InitSpawn();

        void Update();
            // Update -> sub-functions:

        // These utilize compile-time duck-typing. This means objects of any type that has the required members can be passed.
        // They don't need to specificalily inherit from any interface; it's a user-dependent contract (like you'd see in Python) 
        template <typename Pool>
        void UpdateMovement(Pool& pool, int numEntities);

        template <typename Pool, typename Factory, typename SpawnCreator>
        void UpdateSpawning(Pool& pool, Factory& factory, SpawnCreator spawnCreator,
                            int numEntities, bool respawnInactive = true);

        template <typename Pool>
        void UpdateEntityHealth(Pool& pool, int numEntities);

        void UpdateCollisions();

        template <typename CircularEntityPool1, typename Interact1, typename CircularEntityPool2, typename Interact2>
        void CEntityCEntityCollision(CircularEntityPool1& pool1, Interact1 interactor1, CircularEntityPool2& pool2, Interact2 interactor2);

        template <typename CircularEntityPool, typename Interact1, typename RectangularObjectPool, typename Interact2>
        void CEntityRObjectCollision(CircularEntityPool& pool1, Interact1 interactor1, RectangularObjectPool& pool2, Interact2 interactor2);

        void ProcessInput();

        std::vector<HashGrid::EntityPosition> GetActiveEntityPositions() const;

        void ProcessEntitySelection(Vector2 cursor);

        void ProcessSelectedEntity();
        
        template <typename EntityPool>
        int ProcessEntityClick(EntityPool& entityPool, Vector2 cursor);

        // ------------------------
            
        void Render();

        void ManualTestWalls();

        // Entities
        CellPool cellPool;
        FoodPool foodPool;
        RootPool rootPool;

        EntityFactory entityFactory;

        // Entities max
        int maxCells;
        int maxFood;
        int maxRoots; // env


        // AI

        Brain cellBrain;

        // Registry + hashing
        IDRegistry registry;
        HashGrid hashGrid;

        // Environment & UI
        Walls walls;
        Renderer renderer;
        UI gui;

        std::optional<SelectedPool> selectedPool;
        int selectedIndex {-1};
        world::EntityType selectedType {world::EntityType::None};
};
