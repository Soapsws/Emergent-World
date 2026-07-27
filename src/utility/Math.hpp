#pragma once

#include <raylib.h>

namespace math {

    inline float GetRandomFloat(float min, float max) {
        float scale = (float)GetRandomValue(0, 2147483647) / 2147483647.0f;
        return min + scale * (max - min);
    }

}