#include "State.hpp"

#include <cmath>

#include "sim_constants.hpp"

float State::GetWhiskerProximity(const entity::Transform& transform, float angle,
    float whiskerLength,
    const std::function<float(const entity::Transform&, float, float)>& checkRayCollision) const {
    const float facingAngle = std::atan2(facing.y, facing.x);
    const float whiskerAngle = facingAngle + angle;
    const float endX = transform.position.x + whiskerLength * std::cos(whiskerAngle);
    const float endY = transform.position.y + whiskerLength * std::sin(whiskerAngle);
    return checkRayCollision(transform, endX, endY);
}

float State::GetLeftWhiskerProximity(const entity::Transform& transform,
    const std::function<float(const entity::Transform&, float, float)>& checkRayCollision) const {
    return GetWhiskerProximity(transform, -0.5f, 10.0f, checkRayCollision);
}

float State::GetRightWhiskerProximity(const entity::Transform& transform,
    const std::function<float(const entity::Transform&, float, float)>& checkRayCollision) const {
    return GetWhiskerProximity(transform, 0.5f, 10.0f, checkRayCollision);
}

float State::GetCenterWhiskerProximity(const entity::Transform& transform,
    const std::function<float(const entity::Transform&, float, float)>& checkRayCollision) const {
    return GetWhiskerProximity(transform, 0.0f, 14.0f, checkRayCollision);
}

std::vector<float> State::GetNearestFoodTransform(const entity::Transform& transform, 
    const std::function<std::vector<float>(const entity::Transform&, 
    world::EntityType entityType1, world::EntityType entityType2)>& checkFoodProximity) const {

    std::vector<float> nearestFoodTransform = checkFoodProximity(transform, world::EntityType::Cell, world::EntityType::Food);
    return nearestFoodTransform;
}

std::vector<float> State::GetNearestPredatorTransform(const entity::Transform& transform,
    const std::function<std::vector<float>(const entity::Transform&,
    world::EntityType, world::EntityType)>& checkPredatorProximity) const {
    return checkPredatorProximity(transform, world::EntityType::Cell, world::EntityType::Predator);
}
