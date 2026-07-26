#pragma once

#include <raylib.h>
#include <vector> 

struct Walls {

    Walls();

    std::vector<Rectangle> walls;

    bool AddWall(float x, float y, float width, float height);
    bool RemoveWall(int index);

};