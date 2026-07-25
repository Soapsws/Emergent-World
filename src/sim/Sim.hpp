#pragma once

#include "CellPool.hpp"
#include "CellFactory.hpp"
#include "FoodPool.hpp"
#include "FoodFactory.hpp"

class Sim {

    public: 

        // in .hpp the constructor declaration does not include member initializers. 
        Sim();
        ~Sim();

        void Run();

    private:
        void Update();
            // Update -> sub-functions:

        // These utilize compile-time duck-typing. This means objects of any type that has the required members can be passed.
        // They don't need to specificalily inherit from any interface; it's a user-dependent contract (like you'd see in Python) 
        template <typename Pool>
        void UpdateMovement(Pool& pool, int numEntities);

        template <typename Pool, typename Factory, typename SpawnCreator>
        void UpdateSpawning(Pool& pool, Factory& factory, SpawnCreator spawnCreator, int numEntities);

        void UpdateCollisions();

        template <typename CircularEntityPool1, typename Interact1, typename CircularEntityPool2, typename Interact2>
        void CircleCircleCollision(CircularEntityPool1& pool1, Interact1 interactor1, CircularEntityPool2& pool2, Interact2 interactor2);

            // ------------------------
            
        void Render();

        int numCells;
        int numFood;
        CellPool cellPool;
        CellFactory cellFactory;
        FoodPool foodPool;
        FoodFactory foodFactory;

};