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

namespace sim {
}