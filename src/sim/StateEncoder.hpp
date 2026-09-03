#pragma once

#include <vector>
#include "sim_constants.hpp"
#include "CellPool.hpp"
#include "FoodPool.hpp"
#include "RootPool.hpp"
#include "IDRegistry.hpp"
#include "HashGrid.hpp"
#include "Walls.hpp"

class StateEncoder {
    public:
        struct EntityInfo {
            IDRegistry::GlobalID id;
            world::EntityType type;
            entity::Transform transform;
            float radius;
        };

    /*
    Includes all helper functions for state encoding for individual entities and general logging
        Sits under only Sim and main in the dependency hierarchy, so receieves a lot of information

        -- later we can extend this to general data logging for quantitative analysis
    */ 


    public:
        StateEncoder(const CellPool& cells, const FoodPool& food, const RootPool& root, 
            const IDRegistry& registry, const HashGrid& hashGrid, const Walls& walls);
        ~StateEncoder() = default;  
        
        std::vector<EntityInfo> GetInRange(float x, float y, float radius, world::EntityType entityType) const;
        std::vector<EntityInfo> GetFoodInRange(float x, float y, float radius) const;
        std::vector<EntityInfo> GetPredatorsInRange(float x, float y, float radius) const;

        float RaycastCollision(const entity::Transform& transform, float endX, float endY) const;

        std::vector<entity::Transform> GetXClosest(float x, float y,
            std::vector<entity::Transform> transforms, int num) const;

        // most important
        

        
    private:
        const CellPool& cells;
        const FoodPool& food;
        const RootPool& root;
        // add predators later

        const IDRegistry& registry;
        const HashGrid& hashGrid;

        const Walls& walls;

};
