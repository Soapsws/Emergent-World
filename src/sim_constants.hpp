#pragma once
#include <raylib.h>
#include <utility>

namespace settings {
    const int SCREEN_WIDTH = 1500;
    const int SCREEN_HEIGHT = 1000;
}

namespace world {
    const int ENTITY_SPECIES_COUNT = 1;
    const int FOOD_TYPE_COUNT = 1;

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
    struct Transform {
        Vector2 position;
        Vector2 velocity;
    };

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
    const int MAX_CELLS = 100;

    struct CellSpawnConfig {
        world::EntitySpecies species;
        entity::TransformBounds transformBounds; 
        entity::SpawningBounds spawningBounds; 
        Vector2 radiusBounds;   
        Vector2 healthBounds;   
        Vector2 dpsBounds; // new
    };

    inline const CellSpawnConfig DEFAULT {
        world::EntitySpecies::Cell,
        entity::TransformBounds{
            Vector2{ 0.0f, 0.0f },
            Vector2{ static_cast<float>(settings::SCREEN_WIDTH), static_cast<float>(settings::SCREEN_HEIGHT) },
            Vector2{ -1.0f, -1.0f },
            Vector2{ 1.0f, 1.0f }
        }, // transform
        entity::SpawningBounds{ Vector2{ 5.0f, 10.0f }, Vector2{ 5.0f, 10.0f } }, // spawning
        Vector2{ 3.0f, 8.0f },     // radius
        Vector2{ 50.0f, 100.0f }, // health
        Vector2{ 5.0f, 10.0f }      // dps
    };

    struct CellData {
        world::EntitySpecies species; 
        entity::Transform transform; 
        entity::Spawning spawning; 
        float radius;
        float health;
        float dps; // new
        bool active;
    };

    inline cells::CellData defaultSpawn() {
    return {
        world::EntitySpecies::Cell,

        entity::Transform{
            Vector2{
                static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.transformBounds.positionMin.x), static_cast<int>(cells::DEFAULT.transformBounds.positionMax.x))),
                static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.transformBounds.positionMin.y), static_cast<int>(cells::DEFAULT.transformBounds.positionMax.y)))
            },
            Vector2{
                static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.transformBounds.velocityMin.x), static_cast<int>(cells::DEFAULT.transformBounds.velocityMax.x))),
                static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.transformBounds.velocityMin.y), static_cast<int>(cells::DEFAULT.transformBounds.velocityMax.y)))
            }
        },

        entity::Spawning{
            static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.spawningBounds.lifetimeBounds.x), static_cast<int>(cells::DEFAULT.spawningBounds.lifetimeBounds.y))),
            static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.spawningBounds.cooldownBounds.x), static_cast<int>(cells::DEFAULT.spawningBounds.cooldownBounds.y)))
        },

        static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.radiusBounds.x), static_cast<int>(cells::DEFAULT.radiusBounds.y))),
        static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.healthBounds.x), static_cast<int>(cells::DEFAULT.healthBounds.y))),
        0.0f,
        true
    };
    }
}

namespace food {
    const int MAX_NATURAL_FOOD = 30;

    struct FoodSpawnConfig {
        world::FoodTypes type;
        entity::TransformBounds transformBounds;
        entity::SpawningBounds spawningBounds;
        Vector2 radiusBounds;
        Vector2 healthBounds;
    };

    inline const FoodSpawnConfig DEFAULT {
        world::FoodTypes::Food_All,
        entity::TransformBounds{
            Vector2{ 0.0f, 0.0f },
            Vector2{ static_cast<float>(settings::SCREEN_WIDTH), static_cast<float>(settings::SCREEN_HEIGHT) },
            Vector2{ -0.5f, -0.5f },
            Vector2{ 0.5f, 0.5f }
        },
        entity::SpawningBounds{ Vector2{ 5.0f, 10.0f }, Vector2{ 5.0f, 10.0f } },
        Vector2{ 2.0f, 5.0f },
        Vector2{ 10.0f, 20.0f }
    };

    struct FoodData {
        world::FoodTypes type;
        entity::Transform transform;
        entity::Spawning spawning;
        float radius;
        float health;
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
                    static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.transformBounds.velocityMin.x), static_cast<int>(DEFAULT.transformBounds.velocityMax.x))),
                    static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.transformBounds.velocityMin.y), static_cast<int>(DEFAULT.transformBounds.velocityMax.y)))
                }
            },
            entity::Spawning{
                static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.spawningBounds.lifetimeBounds.x), static_cast<int>(DEFAULT.spawningBounds.lifetimeBounds.y))),
                static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.spawningBounds.cooldownBounds.x), static_cast<int>(DEFAULT.spawningBounds.cooldownBounds.y)))
            },
            static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.radiusBounds.x), static_cast<int>(DEFAULT.radiusBounds.y))),
            static_cast<float>(GetRandomValue(static_cast<int>(DEFAULT.healthBounds.x), static_cast<int>(DEFAULT.healthBounds.y))),
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
    constexpr int NEON_COUNT = 15;

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
        /* NEON_CORAL */ Color{ 0xFF, 0x20, 0x20, 0xFF }
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
        "NEON_CORAL"
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