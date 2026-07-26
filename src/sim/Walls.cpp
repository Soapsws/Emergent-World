#include <raylib.h>
#include <vector>

#include "Walls.hpp"
#include "sim_constants.hpp"

Walls::Walls() : walls() {}

bool Walls::AddWall(float x, float y, float width, float height) {
    if (x < 0 || y < 0) return false;
    if (x + width> settings::WORLD_WIDTH || y + height > settings::WORLD_HEIGHT) return false;

    walls.push_back({x, y, width, height});
    return true;
}

bool Walls::RemoveWall(int index) {
    if (index < 0 || index >= walls.size()) return false;

    walls.erase(walls.begin() + index);
    return true;
}