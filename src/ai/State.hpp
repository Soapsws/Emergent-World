#pragma once

#include <raylib.h>
#include <functional>
#include <vector>
#include "EntityTypes.hpp"

namespace entity {
}

namespace world {
    enum class EntityType;
}

struct State {

    entity::Transform transform;

    // agent variables
    float health;
    float hunger;
    float energy; 
    Vector2 facing;

    // const suffix -> promises not to modify the object it is called on

    // to be passed: StateEncoder::RaycastCollision
    float getWhiskerProximity(const entity::Transform&, float, float,
        const std::function<float(const entity::Transform&, float, float)>&) const;
    float getLeftWhiskerProximity(const entity::Transform&,
        const std::function<float(const entity::Transform&, float, float)>&) const;
    float getRightWhiskerProximity(const entity::Transform&,
        const std::function<float(const entity::Transform&, float, float)>&) const;
    float getCenterWhiskerProximity(const entity::Transform&,
        const std::function<float(const entity::Transform&, float, float)>&) const;

    std::vector<float> getNearestFoodTransform(const entity::Transform& transform,
        const std::function<std::vector<float>(const entity::Transform&, 
        world::EntityType entityType1, world::EntityType entityType2)>& checkFoodProximity) const;

    std::vector<float> getNearestPredatorTransform(const entity::Transform& transform,
        const std::function<std::vector<float>(const entity::Transform&,
        world::EntityType entityType1, world::EntityType entityType2)>& checkPredatorProximity) const;

    std::vector<float>& passStatualInput(const entity::Transform& transform,
        const std::function<float(const entity::Transform&, float, float)>& checkRayCollision,
        const std::function<std::vector<float>(const entity::Transform&, 
        world::EntityType entityType1, world::EntityType entityType2)>& checkFoodProximity);


};
