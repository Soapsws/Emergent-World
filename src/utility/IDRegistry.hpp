#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <vector>

#include "sim_constants.hpp"

// The registry is the authority for identity. A global ID identifies a
// logical pool slot; the slot may become inactive and later be reused without
// changing its global ID.
class IDRegistry {
public:
    using GlobalID = int;
    static constexpr GlobalID InvalidID = -1;

    struct EntityRef {
        world::EntityType type {world::EntityType::None};
        int poolIndex {-1};
        bool active {false};
    };

    IDRegistry() = default;

    // Register a pool slot and activate it. Calling this again for the same
    // type/index returns the original ID instead of creating a duplicate.
    GlobalID AddEntity(world::EntityType type, int poolIndex) {
        ValidateType(type);
        assert(poolIndex >= 0);

        auto& slotIDs = idsByType[TypeIndex(type)];
        if (poolIndex >= static_cast<int>(slotIDs.size())) {
            slotIDs.resize(static_cast<std::size_t>(poolIndex) + 1, InvalidID);
        }

        GlobalID id = slotIDs[poolIndex];
        if (id == InvalidID) {
            id = static_cast<GlobalID>(entities.size());
            slotIDs[poolIndex] = id;
            entities.push_back({type, poolIndex, false});
            ++registeredCounts[TypeIndex(type)];
        }

        Activate(id);
        return id;
    }

    void Activate(GlobalID id) {
        EntityRef& entity = Get(id);
        if (!entity.active) {
            entity.active = true;
            ++activeCounts[TypeIndex(entity.type)];
        }
    }

    void Deactivate(GlobalID id) {
        EntityRef& entity = Get(id);
        if (entity.active) {
            entity.active = false;
            --activeCounts[TypeIndex(entity.type)];
        }
    }

    GlobalID Find(world::EntityType type, int poolIndex) const {
        ValidateType(type);
        const auto& slotIDs = idsByType[TypeIndex(type)];
        if (poolIndex < 0 || poolIndex >= static_cast<int>(slotIDs.size())) {
            return InvalidID;
        }
        return slotIDs[poolIndex];
    }

    const EntityRef& Get(GlobalID id) const {
        assert(IsValid(id));
        return entities[static_cast<std::size_t>(id)];
    }

    EntityRef& Get(GlobalID id) {
        assert(IsValid(id));
        return entities[static_cast<std::size_t>(id)];
    }

    bool IsValid(GlobalID id) const {
        return id >= 0 && id < static_cast<GlobalID>(entities.size());
    }

    bool IsActive(GlobalID id) const {
        return IsValid(id) && Get(id).active;
    }

    int RegisteredCount(world::EntityType type) const {
        ValidateType(type);
        return registeredCounts[TypeIndex(type)];
    }

    int ActiveCount(world::EntityType type) const {
        ValidateType(type);
        return activeCounts[TypeIndex(type)];
    }

    int TotalRegistered() const {
        return static_cast<int>(entities.size());
    }

    int TotalActive() const {
        int total = 0;
        for (int count : activeCounts) total += count;
        return total;
    }

private:
    static constexpr std::size_t TypeCount = 5;

    static constexpr std::size_t TypeIndex(world::EntityType type) {
        return static_cast<std::size_t>(type);
    }

    static void ValidateType(world::EntityType type) {
        assert(type != world::EntityType::None);
        assert(TypeIndex(type) < TypeCount);
    }

    std::vector<EntityRef> entities;

    // Maps [entity type][pool index] to the stable global ID.
    std::array<std::vector<GlobalID>, TypeCount> idsByType;
    std::array<int, TypeCount> registeredCounts {};
    std::array<int, TypeCount> activeCounts {};
};
