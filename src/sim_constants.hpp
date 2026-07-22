#pragma once
#include <raylib.h>
#include <utility>

namespace settings {
    const int SCREEN_WIDTH = 1500;
    const int SCREEN_HEIGHT = 1000;
}

namespace cells {
    const int MAX_CELLS = 100;
    const float CELL_RADIUS = 5.0f;

    struct CellSpawnConfig {
        Vector2 velocityBounds; 
        Vector2 healthBounds;   
        Vector2 lifetimeBounds; 
    };

    inline const CellSpawnConfig DEFAULT {
        Vector2{ -1.0f, 1.0f },   
        Vector2{ 50.0f, 100.0f }, 
        Vector2{ 50.0f, 100.0f }  
    };

    struct CellData {
        Vector2 position;
        Vector2 velocity;
        float health;
        float lifetime;
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

            static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.healthBounds.x), static_cast<int>(cells::DEFAULT.healthBounds.y))),
            static_cast<float>(GetRandomValue(static_cast<int>(cells::DEFAULT.lifetimeBounds.x), static_cast<int>(cells::DEFAULT.lifetimeBounds.y))),
            true
        };
    }
}