#pragma once

#include <raylib.h>
#include <vector> 

#include "sim_constants.hpp"

struct Walls {

    Walls();

    std::vector<Rectangle> walls;

    // Rectangle shadow data to conform to SoA.
    std::vector<entity::Transform> transform;
    std::vector<float> width;
    std::vector<float> height;

    bool AddWall(float x, float y, float width, float height);
    bool RemoveWall(int index);

};
