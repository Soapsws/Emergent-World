#include "StateEncoder.hpp"

StateEncoder::StateEncoder(const CellPool& cells, const FoodPool& food,
                           const RootPool& root, const IDRegistry& registry,
                           const HashGrid& hashGrid,
                           const Walls& walls)
    : cells(cells), food(food), root(root), registry(registry), hashGrid(hashGrid), walls(walls) {}

std::vector<StateEncoder::EntityInfo> StateEncoder::GetInRange(
    float x, float y, float radius, world::EntityType entityType) const {
    std::vector<EntityInfo> entities;

    for (const IDRegistry::GlobalID id : hashGrid.CheckInteractions(x, y)) {
        const IDRegistry::EntityRef& entityRef = registry.Get(id);
        if ((entityType != world::EntityType::Any && entityRef.type != entityType)
            || !entityRef.active) continue;

        const int index = entityRef.poolIndex;

        if (entityRef.type == world::EntityType::Cell) {
            if (index >= 0 && index < static_cast<int>(cells.active.size()) && cells.active[index]) {
                if (math::IsInRange(x, y, cells.state[index].transform.position.x, cells.state[index].transform.position.y, radius)) {
                    entities.push_back({id, entityRef.type, cells.state[index].transform, cells.radius[index]});
                }
            }
        } else if (entityRef.type == world::EntityType::Food) {
            if (index >= 0 && index < static_cast<int>(food.active.size()) && food.active[index]) {
                if (math::IsInRange(x, y, food.transform[index].position.x, food.transform[index].position.y, radius)) {
                    entities.push_back({id, entityRef.type, food.transform[index], food.radius[index]});
                }
            }
        } else if (entityRef.type == world::EntityType::Root) {
            if (index >= 0 && index < static_cast<int>(root.active.size()) && root.active[index]) {
                if (math::IsInRange(x, y, root.transform[index].position.x, root.transform[index].position.y, radius)) {
                    entities.push_back({id, entityRef.type, root.transform[index], root.radius[index]});
                }
            }
        }
    }

    return entities;
}

std::vector<StateEncoder::EntityInfo> StateEncoder::GetFoodInRange(float x, float y, float radius) const {
    return GetInRange(x, y, radius, world::EntityType::Food);
}

std::vector<StateEncoder::EntityInfo> StateEncoder::GetPredatorsInRange(float x, float y, float radius) const {
    return GetInRange(x, y, radius, world::EntityType::Predator);
}

std::vector<entity::Transform> StateEncoder::GetXClosest(
    float x, float y, std::vector<entity::Transform> transforms, int num) const {
    std::sort(transforms.begin(), transforms.end(), [x, y](const entity::Transform& a, const entity::Transform& b) {
        const float distA = math::DistanceSquared(x, y, a.position.x, a.position.y);
        const float distB = math::DistanceSquared(x, y, b.position.x, b.position.y);
        return distA < distB;
    });
    if (num < 0) {
        num = 0;
    }
    if (num < static_cast<int>(transforms.size())) {
        transforms.resize(num);
    }
    return transforms;
}

float StateEncoder::RaycastCollision(const entity::Transform& transform, float endX, float endY) const {
    /* 
    uses optimize hashgrid search for entities but naive global search for walls
        because it's anticipated that the number of walls will be relatively small
        and walls can span numerous cells
    */

    float closestIntersection = std::numeric_limits<float>::max();
    
    std::vector<EntityInfo> entitiesInRange = GetInRange(transform.position.x, transform.position.y, math::Distance(transform.position.x, transform.position.y, endX, endY), world::EntityType::Any);
    for (const EntityInfo& entity : entitiesInRange) {
        // currently only circular entities exist; add separate functionality for rectangular/miscellaneous entities later if necessary
        const float entityRadius = entity.radius;
        std::pair<float, float> intersection = math::RaycastCircleIntersection(transform.position.x, transform.position.y, endX, endY, entity.transform.position.x, entity.transform.position.y, entityRadius);
        if (intersection.first >= 0.0f && intersection.second >= 0.0f) {
            float distance = math::Distance(transform.position.x, transform.position.y, intersection.first, intersection.second);
            if (distance < closestIntersection) {
                closestIntersection = distance;
            }
        }
    }

    for (int i = 0; i < walls.walls.size(); i++) {
        const auto& wall = walls.walls[i];
        std::pair<float, float> intersection = math::RaycastRectangleIntersection(transform.position.x, transform.position.y, endX, endY, wall.x, wall.y, wall.width, wall.height);
        if (intersection.first >= 0.0f && intersection.second >= 0.0f) {
            float distance = math::Distance(transform.position.x, transform.position.y, intersection.first, intersection.second);
            if (distance < closestIntersection) {
                closestIntersection = distance;
            }
        }
    }

    return closestIntersection;
}
 
