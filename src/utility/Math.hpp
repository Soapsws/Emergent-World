#pragma once

#include <vector>
#include <raylib.h>
#include <cmath>
#include <algorithm>

namespace math {

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

}