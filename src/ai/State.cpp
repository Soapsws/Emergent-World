#include "State.hpp"

#include <cmath>

#include "sim_constants.hpp"

float State::getWhiskerWallProximity(const entity::Transform& transform, float angle,
    float whiskerLength,
    const std::function<float(const entity::Transform&, float, float)>& checkRayCollision) const {
    const float facingAngle = std::atan2(facing.y, facing.x);
    const float whiskerAngle = facingAngle + angle;
    const float endX = transform.position.x + whiskerLength * std::cos(whiskerAngle);
    const float endY = transform.position.y + whiskerLength * std::sin(whiskerAngle);
    return checkRayCollision(transform, endX, endY);
}

float State::getLeftWhiskerWallProximity(const entity::Transform& transform,
    const std::function<float(const entity::Transform&, float, float)>& checkRayCollision) const {
    return getWhiskerWallProximity(transform, -0.5f, 10.0f, checkRayCollision);
}

float State::getRightWhiskerWallProximity(const entity::Transform& transform,
    const std::function<float(const entity::Transform&, float, float)>& checkRayCollision) const {
    return getWhiskerWallProximity(transform, 0.5f, 10.0f, checkRayCollision);
}

float State::getCenterWhiskerWallProximity(const entity::Transform& transform,
    const std::function<float(const entity::Transform&, float, float)>& checkRayCollision) const {
    return getWhiskerWallProximity(transform, 0.0f, 14.0f, checkRayCollision);
}

std::vector<float> State::getNearestFoodTransform(const entity::Transform& transform, 
    const std::function<std::vector<float>(const entity::Transform&, 
    world::EntityType entityType1, world::EntityType entityType2)>& checkFoodProximity) const {

    std::vector<float> nearestFoodTransform = checkFoodProximity(transform, world::EntityType::Cell, world::EntityType::Food);
    return nearestFoodTransform;
}

std::vector<float> State::getNearestPredatorTransform(const entity::Transform& transform,
    const std::function<std::vector<float>(const entity::Transform&,
    world::EntityType, world::EntityType)>& checkPredatorProximity) const {
    return checkPredatorProximity(transform, world::EntityType::Cell, world::EntityType::Predator);
}


std::vector<float>& State::passStatualInput(const entity::Transform& transform,
    const std::function<float(const entity::Transform&, float, float)>& checkRayCollision,
    const std::function<std::vector<float>(const entity::Transform&, 
    world::EntityType entityType1, world::EntityType entityType2)>& checkFoodProximity) {
    static std::vector<float> input;
    input.clear();

    std::vector<float> nearestFoodTransform = getNearestFoodTransform(transform, checkFoodProximity);

    std::vector<float> nearestPredatorTransform = getNearestPredatorTransform(transform, checkFoodProximity);

    // The callback contract is expected to return three values:
    // distance, sin(angle), and cos(angle).
    input.insert(input.end(), nearestFoodTransform.begin(), nearestFoodTransform.end());
    input.insert(input.end(), nearestPredatorTransform.begin(), nearestPredatorTransform.end());

    input.push_back(getLeftWhiskerWallProximity(transform, checkRayCollision));
    input.push_back(getRightWhiskerWallProximity(transform, checkRayCollision));
    input.push_back(getCenterWhiskerWallProximity(transform, checkRayCollision));

    input.push_back(health);
    input.push_back(energy);
    input.push_back(hunger);
 
    return input;
}
