#include "StateEncoder.hpp"

StateEncoder::StateEncoder(const CellPool& cells, const FoodPool& food,
                           const RootPool& root, const IDRegistry& registry,
                           const HashGrid& hashGrid)
    : cells(cells), food(food), root(root), registry(registry), hashGrid(hashGrid) {}

std::vector<entity::Transform> StateEncoder::GetInRange(
    float x, float y, float radius, world::EntityType entityType) {
    std::vector<entity::Transform> transforms;

    for (const IDRegistry::GlobalID id : hashGrid.CheckInteractions(x, y)) {
        const IDRegistry::EntityRef& entityRef = registry.Get(id);
        if (entityRef.type != entityType || !entityRef.active) continue;

        const int index = entityRef.poolIndex;

        if (entityType == world::EntityType::Cell) {
            if (index >= 0 && index < static_cast<int>(cells.active.size()) && cells.active[index]) {
                if (math::IsInRange(x, y, cells.state[index].transform.position.x, cells.state[index].transform.position.y, radius)) {
                    transforms.push_back(cells.state[index].transform);
                }
            }
        } else if (entityType == world::EntityType::Food) {
            if (index >= 0 && index < static_cast<int>(food.active.size()) && food.active[index]) {
                if (math::IsInRange(x, y, food.transform[index].position.x, food.transform[index].position.y, radius)) {
                    transforms.push_back(food.transform[index]);
                }
            }
        } else if (entityType == world::EntityType::Root) {
            if (index >= 0 && index < static_cast<int>(root.active.size()) && root.active[index]) {
                if (math::IsInRange(x, y, root.transform[index].position.x, root.transform[index].position.y, radius)) {
                    transforms.push_back(root.transform[index]);
                }
            }
        }
    }

    return transforms;
}

std::vector<entity::Transform> StateEncoder::GetFoodInRange(float x, float y, float radius) {
    return GetInRange(x, y, radius, world::EntityType::Food);
}

std::vector<entity::Transform> StateEncoder::GetPredatorsInRange(float x, float y, float radius) {
    return GetInRange(x, y, radius, world::EntityType::Predator);
}

std::vector<entity::Transform> StateEncoder::GetXClosest(
    float x, float y, std::vector<entity::Transform> transforms, int num) {
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
