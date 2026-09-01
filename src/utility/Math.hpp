#pragma once

#include <vector>
#include <raylib.h>
#include <cmath>
#include <algorithm>

namespace math {

    inline float DistanceSquared(float x1, float y1, float x2, float y2) {
        const float dx = x2 - x1;
        const float dy = y2 - y1;
        return dx * dx + dy * dy;
    }

    inline float GetRandomFloat(float min, float max) {
        float scale = (float)GetRandomValue(0, 2147483647) / 2147483647.0f;
        return min + scale * (max - min);
    }

    inline void relu(std::vector<float>& layer) {
        for (auto& neuron : layer) {
            neuron = std::max(0.0f, neuron);
        }
    }

    inline void tanh(std::vector<float>& layer) {
        for (auto& neuron : layer) {
            neuron = std::tanh(neuron);
        }
    }

    inline bool IsInRange(float x1, float y1, float x2, float y2, float radius) {
        return DistanceSquared(x1, y1, x2, y2) <= radius * radius;
    }

}
