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
        void Render();

        int numCells;
        int numFood;
        CellPool cellPool;
        CellFactory cellFactory;
        FoodPool foodPool;
        FoodFactory foodFactory;

};