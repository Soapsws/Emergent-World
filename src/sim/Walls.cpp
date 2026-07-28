#include <raylib.h>
#include <vector>

#include "Walls.hpp"
#include "sim_constants.hpp"

Walls::Walls() : walls(), transform(), width(), height() {}

bool Walls::AddWall(float x, float y, float width, float height) {
    if (x < 0 || y < 0 || width <= 0 || height <= 0) return false;
    if (x + width > settings::WORLD_WIDTH || y + height > settings::WORLD_HEIGHT) return false;

    walls.push_back({x, y, width, height});

    transform.push_back({
        Vector2{ x, y },
        Vector2{ 0.0f, 0.0f }
    });
    this->width.push_back(width);
    this->height.push_back(height);

    return true;
}

bool Walls::RemoveWall(int index) {
    if (index < 0 || index >= walls.size()) return false;

    walls.erase(walls.begin() + index);

    transform.erase(transform.begin() + index);
    width.erase(width.begin() + index);
    height.erase(height.begin() + index);

    return true;
}
