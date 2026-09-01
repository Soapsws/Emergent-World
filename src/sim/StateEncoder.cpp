#include "StateEncoder.hpp"



std::vector<entity::Transform> StateEncoder::getXNearest(float x, float y, float radius, world::EntityType entityType) {
    std::vector<IDRegistry::GlobalID> IDsInRange;
    for (const auto& id : hashGrid.CheckInteractions(x, y)) {
        const auto& entityRef = registry.Get(id);
        if (entityRef.type == entityType &&
            entityRef.
            ) {



        }      
    }

    return IDsInRange;
}