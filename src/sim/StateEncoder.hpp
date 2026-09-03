#pragma once

#include <raylib.h>
#include <raymath.h>

#include <vector>
#include <cstddef>
#include <limits>
#include "sim_constants.hpp"
#include "CellPool.hpp"
#include "FoodPool.hpp"
#include "RootPool.hpp"
#include "IDRegistry.hpp"
#include "HashGrid.hpp"
#include "Walls.hpp"
#include "Math.hpp"

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
        template <typename StatefulEntityPool>
        std::vector<float> AggregateData(const StatefulEntityPool& entityPool, int i) const {
            // This is deliberately explicit for now: food and predator sensing
            // are parallel examples that can be generalized later.
            std::vector<float> data;
            const State& state = entityPool.state[i];
            const Vector2 position = state.transform.position;
            const float sensoryRadius = entityPool.visionRadius[i];

            std::vector<EntityInfo> nearbyFood = GetFoodInRange(position.x, position.y, sensoryRadius);
            std::vector<entity::Transform> nearbyFoodTransforms;
            for (const EntityInfo& entity : nearbyFood) nearbyFoodTransforms.push_back(entity.transform);
            const auto closestFood = GetXClosest(position.x, position.y, nearbyFoodTransforms, 1);

            float foodDistance = sensoryRadius;
            float foodAngle = 0.0f;
            if (!closestFood.empty()) {
                const Vector2 toFood = Vector2Subtract(closestFood.front().position, position);
                foodDistance = Vector2Length(toFood);
                foodAngle = math::SignedAngleBetweenVectors(state.facing, Vector2Normalize(toFood));
            }
            data.push_back(foodDistance);
            data.push_back(std::sin(foodAngle));
            data.push_back(std::cos(foodAngle));

            std::vector<EntityInfo> nearbyPredators = GetPredatorsInRange(position.x, position.y, sensoryRadius);
            std::vector<entity::Transform> nearbyPredatorTransforms;
            for (const EntityInfo& entity : nearbyPredators) nearbyPredatorTransforms.push_back(entity.transform);
            const auto closestPredator = GetXClosest(position.x, position.y, nearbyPredatorTransforms, 1);
            float predatorDistance = sensoryRadius;
            float predatorAngle = 0.0f;
            if (!closestPredator.empty()) {
                const Vector2 toPredator = Vector2Subtract(closestPredator.front().position, position);
                predatorDistance = Vector2Length(toPredator);
                predatorAngle = math::SignedAngleBetweenVectors(state.facing, Vector2Normalize(toPredator));
            }
            data.push_back(predatorDistance);
            data.push_back(std::sin(predatorAngle));
            data.push_back(std::cos(predatorAngle));

            const auto ray = [&](float offset, float length) {
                const float facingAngle = std::atan2(state.facing.y, state.facing.x) + offset;
                return RaycastCollision(state.transform,
                    position.x + length * std::cos(facingAngle),
                    position.y + length * std::sin(facingAngle));
            };
            data.push_back(ray(-0.5f, 10.0f));
            data.push_back(ray(0.0f, 14.0f));
            data.push_back(ray(0.5f, 10.0f));
            data.push_back(state.health);
            data.push_back(state.energy);
            data.push_back(state.hunger);
            return data;
        }
        

        
    private:
        const CellPool& cells;
        const FoodPool& food;
        const RootPool& root;
        // add predators later

        const IDRegistry& registry;
        const HashGrid& hashGrid;

        const Walls& walls;

};
