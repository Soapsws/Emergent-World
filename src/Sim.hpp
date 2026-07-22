#pragma once

#include "CellPool.hpp"
#include "CellFactory.hpp"

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
        CellPool cellPool;
        CellFactory cellFactory;

};