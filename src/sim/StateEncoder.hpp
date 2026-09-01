#pragma once

#include <vector>
#include "sim_constants.hpp"
#include "CellPool.hpp"
#include "FoodPool.hpp"
#include "RootPool.hpp"
#include "IDRegistry.hpp"
#include "HashGrid.hpp"
#include "TransformAccess.hpp"

class StateEncoder {

    /*
    Includes all helper functions for state encoding for individual entities and general logging
        Sits under only Sim and main in the dependency hierarchy, so receieves a lot of information

        -- later we can extend this to general data logging for quantitative analysis
    */ 


    public:
        StateEncoder(const CellPool& cells, const FoodPool& food, const RootPool& root, 
            const IDRegistry& registry, const HashGrid& hashGrid);
        ~StateEncoder() = default;  
        
        std::vector<entity::Transform> getXNearest(float x, float y, float radius, world::EntityType entityType);
        std::vector<entity::Transform> getXNearestFood(float x, float y, float radius);
        std::vector<entity::Transform> getXNearestPredator(float x, float y, float radius);

        
    private:
        const CellPool& cells;
        const FoodPool& food;
        const RootPool& root;
        // add predators later

        const IDRegistry& registry;
        const HashGrid& hashGrid;
};
