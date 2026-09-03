#pragma once
#include <raylib.h>
#include <utility>

#include "Math.hpp"
#include "EntityTypes.hpp"
#include "State.hpp"

namespace settings {
    const int SCREEN_WIDTH = 1500;
    const int SCREEN_HEIGHT = 1000;

    const int WORLD_WIDTH = 9000;
    const int WORLD_HEIGHT = 6000;
    const int WORLD_EDGE_SPAWN_OFFSET = 100;
}

namespace util {
    constexpr int HASH_TABLE_SIZE = 1024;
}

namespace world {
    const int ENTITY_SPECIES_COUNT = 1;
    const int FOOD_TYPE_COUNT = 1;

    enum class EntityType { None, Cell, Food, Root, Predator, Any };

    enum class EntitySpecies {
        Cell, // (a.k.a CELL1)
        // INERT_MATTER (see IOM)
        // CELL2
    };

    enum class FoodTypes {
        Food_All,
        // FOOD_CARN,
        // FOOD_HERB,
    };
}

namespace entity {
    struct TransformBounds {
        Vector2 positionMin;
        Vector2 positionMax;
        Vector2 velocityMin;
        Vector2 velocityMax;
    };

    struct Spawning {
        float lifetime;
        float cooldown;
    };

    struct SpawningBounds {
        Vector2 lifetimeBounds;
        Vector2 cooldownBounds;
    };
}

namespace cells {
    const int MAX_CELLS = 250;

    struct CellSpawnConfig {
        world::EntitySpecies species;
        entity::TransformBounds transformBounds; 
        entity::SpawningBounds spawningBounds; 
        Vector2 radiusBounds;   
        Vector2 healthBounds;   
        Vector2 dpsBounds; // new
        Vector2 dragBounds;
        Vector2 facingAngleBounds;
        Vector2 visionRadiusBounds;
    };

    inline const CellSpawnConfig DEFAULT {
        world::EntitySpecies::Cell,
        entity::TransformBounds{
            Vector2{ static_cast<float>(settings::WORLD_EDGE_SPAWN_OFFSET), static_cast<float>(settings::WORLD_EDGE_SPAWN_OFFSET) },
            Vector2{ static_cast<float>(settings::WORLD_WIDTH - settings::WORLD_EDGE_SPAWN_OFFSET), static_cast<float>(settings::WORLD_HEIGHT - settings::WORLD_EDGE_SPAWN_OFFSET) },
            Vector2{ -1.0f, -1.0f },
            Vector2{ 1.0f, 1.0f }
        }, // transform
        entity::SpawningBounds{ Vector2{ 50.0f, 100.0f }, Vector2{ 5.0f, 10.0f } }, // spawning <lifetime, cooldown>
        Vector2{ 6.0f, 16.0f },     // radius
        Vector2{ 50.0f, 100.0f }, // health
        Vector2{ 100.0f, 200.0f },     // dps
        Vector2{ 0.0f, 0.1f },        // drag
        Vector2{ 0.0f, 2.0f * PI },   // facing angle, radians
        Vector2{ 100.0f, 250.0f }     // vision radius
    };

    struct CellData {
        world::EntitySpecies species; 
        State state;
        entity::Spawning spawning; 
        float radius;
        float dps; // new
        float drag;
        float visionRadius;
        bool active;
    };

    inline cells::CellData defaultSpawn() {
    const float facingAngle = math::GetRandomFloat(DEFAULT.facingAngleBounds.x, DEFAULT.facingAngleBounds.y);
    return {
        world::EntitySpecies::Cell,

        State{
            entity::Transform{
                Vector2{
                static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.transformBounds.positionMin.x), static_cast<int>(cells::DEFAULT.transformBounds.positionMax.x))),
                static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.transformBounds.positionMin.y), static_cast<int>(cells::DEFAULT.transformBounds.positionMax.y)))
                },
                Vector2{
                math::GetRandomFloat(cells::DEFAULT.transformBounds.velocityMin.x, cells::DEFAULT.transformBounds.velocityMax.x),
                math::GetRandomFloat(cells::DEFAULT.transformBounds.velocityMin.y, cells::DEFAULT.transformBounds.velocityMax.y)
                }
            },
            math::GetRandomFloat(DEFAULT.healthBounds.x, DEFAULT.healthBounds.y),
            0.0f,
            0.0f,
            Vector2{
                cosf(facingAngle),
                sinf(facingAngle)
            }
        },

        entity::Spawning{
            static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.spawningBounds.lifetimeBounds.x), static_cast<int>(cells::DEFAULT.spawningBounds.lifetimeBounds.y))),
            static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.spawningBounds.cooldownBounds.x), static_cast<int>(cells::DEFAULT.spawningBounds.cooldownBounds.y)))
        },

        math::GetRandomFloat(DEFAULT.radiusBounds.x, DEFAULT.radiusBounds.y),
        math::GetRandomFloat(DEFAULT.dpsBounds.x, DEFAULT.dpsBounds.y),
        math::GetRandomFloat(DEFAULT.dragBounds.x, DEFAULT.dragBounds.y),
        math::GetRandomFloat(DEFAULT.visionRadiusBounds.x, DEFAULT.visionRadiusBounds.y),
        true
    };
    }
}

namespace food {
    const int MAX_NATURAL_FOOD = 120;

    struct FoodSpawnConfig {
        world::FoodTypes type;
        entity::TransformBounds transformBounds;
        entity::SpawningBounds spawningBounds;
        Vector2 radiusBounds;
        Vector2 healthBounds;
        Vector2 dragBounds;
    };

    inline const FoodSpawnConfig DEFAULT {
        world::FoodTypes::Food_All,
        entity::TransformBounds{
            Vector2{ static_cast<float>(settings::WORLD_EDGE_SPAWN_OFFSET), static_cast<float>(settings::WORLD_EDGE_SPAWN_OFFSET) },
            Vector2{ static_cast<float>(settings::WORLD_WIDTH - settings::WORLD_EDGE_SPAWN_OFFSET), 
                static_cast<float>(settings::WORLD_HEIGHT - settings::WORLD_EDGE_SPAWN_OFFSET) },
            Vector2{ -0.5f, -0.5f },
            Vector2{ 0.5f, 0.5f }
        },
        entity::SpawningBounds{ Vector2{ 50.0f, 100.0f }, Vector2{ 5.0f, 10.0f } },
        Vector2{ 5.0f, 10.0f }, // radius
        Vector2{ 0.5f, 0.8f }, // hp (TESTING VALUES)
        Vector2{ 0.02f, 0.06f } // drag
    };

    struct FoodData {
        world::FoodTypes type;
        entity::Transform transform;
        entity::Spawning spawning;
        float radius;
        float health;
        float drag;
        bool active;
        // add more e.g. hunger restored, energy gained, exp gained, etc.
    };

    inline FoodData defaultSpawn() {
        return {
            world::FoodTypes::Food_All,
            entity::Transform{
                Vector2{
                    static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.transformBounds.positionMin.x), static_cast<int>(DEFAULT.transformBounds.positionMax.x))),
                    static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.transformBounds.positionMin.y), static_cast<int>(DEFAULT.transformBounds.positionMax.y)))
                },
                Vector2{
                    math::GetRandomFloat(DEFAULT.transformBounds.velocityMin.x, DEFAULT.transformBounds.velocityMax.x),
                    math::GetRandomFloat(DEFAULT.transformBounds.velocityMin.y, DEFAULT.transformBounds.velocityMax.y)
                }
            },
            entity::Spawning{
                static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.spawningBounds.lifetimeBounds.x), static_cast<int>(DEFAULT.spawningBounds.lifetimeBounds.y))),
                static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.spawningBounds.cooldownBounds.x), static_cast<int>(DEFAULT.spawningBounds.cooldownBounds.y)))
            },
            math::GetRandomFloat(DEFAULT.radiusBounds.x, DEFAULT.radiusBounds.y),
            math::GetRandomFloat(DEFAULT.healthBounds.x, DEFAULT.healthBounds.y),
            math::GetRandomFloat(DEFAULT.dragBounds.x, DEFAULT.dragBounds.y),
            true
        };
    }
}

namespace roots {
    const int MAX_ROOTS = 30;

    struct RootSpawnConfig {
        entity::TransformBounds transformBounds;
        entity::SpawningBounds spawningBounds;
        Vector2 radiusBounds;
        Vector2 spawnRateBounds;
    };

    inline const RootSpawnConfig DEFAULT {
        entity::TransformBounds{
            Vector2{ static_cast<float>(settings::WORLD_EDGE_SPAWN_OFFSET), static_cast<float>(settings::WORLD_EDGE_SPAWN_OFFSET) },
            Vector2{ static_cast<float>(settings::WORLD_WIDTH - settings::WORLD_EDGE_SPAWN_OFFSET), static_cast<float>(settings::WORLD_HEIGHT - settings::WORLD_EDGE_SPAWN_OFFSET) },
            Vector2{ 0.0f, 0.0f },
            Vector2{ 0.0f, 0.0f }
        },
        entity::SpawningBounds{
            Vector2{ 100.0f, 200.0f },
            Vector2{ 10.0f, 20.0f }
        },
        Vector2{ 40.0f, 65.0f }, // radius
        Vector2{ 1.0f, 3.0f } // spawning
    };

    struct RootData {
        entity::Transform transform;
        entity::Spawning spawning;
        float radius;
        int spawnRate;
        bool active;
    };

    inline RootData defaultSpawn() {
        return {
            entity::Transform{
                Vector2{
                    static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.transformBounds.positionMin.x), static_cast<int>(DEFAULT.transformBounds.positionMax.x))),
                    static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.transformBounds.positionMin.y), static_cast<int>(DEFAULT.transformBounds.positionMax.y)))
                },
                Vector2{ 0.0f, 0.0f }
            },
            entity::Spawning{
                static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.spawningBounds.lifetimeBounds.x), static_cast<int>(DEFAULT.spawningBounds.lifetimeBounds.y))),
                static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.spawningBounds.cooldownBounds.x), static_cast<int>(DEFAULT.spawningBounds.cooldownBounds.y)))
            },
            math::GetRandomFloat(DEFAULT.radiusBounds.x, DEFAULT.radiusBounds.y),
            GetRandomValue(static_cast<int>(DEFAULT.spawnRateBounds.x), static_cast<int>(DEFAULT.spawnRateBounds.y)),
            true
        };
    }

}

namespace colors {

    // COLOR PALETTE

    constexpr int NEON_MAGENTA_STD = 0;
    constexpr int NEON_PURPLE = 1;
    constexpr int NEON_ORANGE = 2;
    constexpr int NEON_SKYBLUE = 3;
    constexpr int NEON_LIME = 4;
    constexpr int NEON_PINK = 5;
    constexpr int NEON_CYAN = 6;
    constexpr int NEON_MAGENTA = 7;
    constexpr int NEON_TEAL = 8;
    constexpr int NEON_GOLD = 9;
    constexpr int NEON_ULTRA_PURPLE = 10;
    constexpr int NEON_AZURE = 11;
    constexpr int NEON_HOT_PINK = 12;
    constexpr int NEON_CHARTREUSE = 13;
    constexpr int NEON_CORAL = 14;
    constexpr int NEON_WHITE = 15;
    constexpr int NEON_COUNT = 16;

    inline const Color NEON_COLORS[NEON_COUNT] = {
        /* NEON_MAGENTA_STD */ Color{ 255, 0, 255, 255 },
        /* NEON_PURPLE */ Color{ 200, 122, 255, 255 },
        /* NEON_ORANGE */ Color{ 255, 161, 0, 255 },
        /* NEON_SKYBLUE */ Color{ 102, 191, 255, 255 },
        /* NEON_LIME */ Color{ 0, 158, 47, 255 },
        /* NEON_PINK */ Color{ 255, 109, 194, 255 },
        /* NEON_CYAN */ Color{ 0x00, 0xFF, 0xFF, 0xFF },
        /* NEON_MAGENTA */ Color{ 0xFF, 0x00, 0xFF, 0xFF },
        /* NEON_TEAL */ Color{ 0x00, 0xFF, 0x80, 0xFF },
        /* NEON_GOLD */ Color{ 0xFF, 0xA5, 0x00, 0xFF },
        /* NEON_ULTRA_PURPLE */ Color{ 0x7F, 0x00, 0xFF, 0xFF },
        /* NEON_AZURE */ Color{ 0x00, 0xB0, 0xFF, 0xFF },
        /* NEON_HOT_PINK */ Color{ 0xFF, 0x3F, 0x80, 0xFF },
        /* NEON_CHARTREUSE */ Color{ 0x7C, 0xFF, 0x00, 0xFF },
        /* NEON_CORAL */ Color{ 0xFF, 0x20, 0x20, 0xFF },
        /* NEON_WHITE */ Color{ 0xD8, 0xF2, 0xFF, 0xFF }
    };

    inline const char* NEON_COLOR_NAMES[NEON_COUNT] = {
        "NEON_MAGENTA_STD",
        "NEON_PURPLE",
        "NEON_ORANGE",
        "NEON_SKYBLUE",
        "NEON_LIME",
        "NEON_PINK",
        "NEON_CYAN",
        "NEON_MAGENTA",
        "NEON_TEAL",
        "NEON_GOLD",
        "NEON_ULTRA_PURPLE",
        "NEON_AZURE",
        "NEON_HOT_PINK",
        "NEON_CHARTREUSE",
        "NEON_CORAL",
        "NEON_WHITE"
    };

    inline Color GetNeonColor(int id) {
        if (id < 0) id = 0;
        if (id >= NEON_COUNT) id = id % NEON_COUNT;
        return NEON_COLORS[id];
    }

    inline Color GetNeonColorByIndex(int idx) {
        const int n = NEON_COUNT;
        if (n == 0) return WHITE;
        int i = idx % n;
        if (i < 0) i += n;
        return NEON_COLORS[i];
    }

    inline const char* GetNeonColorName(int id) {
        if (id < 0) id = 0;
        if (id >= NEON_COUNT) id = id % NEON_COUNT;
        return NEON_COLOR_NAMES[id];
    }
    
}
