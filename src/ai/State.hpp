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

    float getWhiskerWallProximity(const entity::Transform&, float, float,
        const std::function<float(const entity::Transform&, float, float)>&) const;
    float getLeftWhiskerWallProximity(const entity::Transform&,
        const std::function<float(const entity::Transform&, float, float)>&) const;
    float getRightWhiskerWallProximity(const entity::Transform&,
        const std::function<float(const entity::Transform&, float, float)>&) const;
    float getCenterWhiskerWallProximity(const entity::Transform&,
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

template <typename Pool>
decltype(auto) TransformAt(Pool& pool, int index) {
    if constexpr (requires { pool.state[index].transform; }) {
        return (pool.state[index].transform);
    } else {
        return (pool.transform[index]);
    }
}
