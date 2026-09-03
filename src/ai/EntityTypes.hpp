#pragma once

#include <raylib.h>

namespace entity {
    struct Transform {
        Vector2 position;
        Vector2 velocity;
        float angularVelocity = 0.0f;
    };
}
