#pragma once
#include <raylib.h>
#include <utility>

namespace settings {
    const int SCREEN_WIDTH = 1500;
    const int SCREEN_HEIGHT = 1000;
}

namespace cells {
    const int MAX_CELLS = 100;

    struct CellSpawnConfig {
        Vector2 velocityBounds; 
        Vector2 radiusBounds;    // new
        Vector2 healthBounds;   
        Vector2 lifetimeBounds; 
        Vector2 cooldownBounds;  // new
    };

    inline const CellSpawnConfig DEFAULT {
        Vector2{ -1.0f, 1.0f },   // velocity
        Vector2{ 3.0f, 8.0f },     // radius
        Vector2{ 50.0f, 100.0f }, // health
        Vector2{ 5.0f, 10.0f },    // lifetime
        Vector2{ 5.0f, 10.0f }   // cooldown
    };

    struct CellData {
        Vector2 position;
        Vector2 velocity;
        float radius;
        float health;
        float lifetime;
        float cooldown;
        bool active;
    };
}

namespace sim {
    inline cells::CellData defaultSpawn() {
        return {
            Vector2{
                static_cast<float>(GetRandomValue(0, settings::SCREEN_WIDTH)),
                static_cast<float>(GetRandomValue(0, settings::SCREEN_HEIGHT))
            },

            Vector2{
                static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.velocityBounds.x), static_cast<int>(cells::DEFAULT.velocityBounds.y))),
                static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.velocityBounds.x), static_cast<int>(cells::DEFAULT.velocityBounds.y)))
            },

            static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.radiusBounds.x), static_cast<int>(cells::DEFAULT.radiusBounds.y))),
            static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.healthBounds.x), static_cast<int>(cells::DEFAULT.healthBounds.y))),
            static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.lifetimeBounds.x), static_cast<int>(cells::DEFAULT.lifetimeBounds.y))),
            static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.cooldownBounds.x), static_cast<int>(cells::DEFAULT.cooldownBounds.y))),
            true
        };
    }
}