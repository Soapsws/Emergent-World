#pragma once

#include <cmath>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <raylib.h>

#include "IDRegistry.hpp"

struct HashGrid {

    HashGrid(int tableSize) : tableSize(tableSize), cellSize(std::max(1, tableSize / 5)) {
        head.assign(tableSize, -1); 
    }
    ~HashGrid() = default;

    int GenerateHash(int x, int y) const {
        const std::uint32_t ux = static_cast<std::uint32_t>(x);
        const std::uint32_t uy = static_cast<std::uint32_t>(y);
        const std::uint32_t h = (ux * 92837111u) ^ (uy * 689287499u);
        return static_cast<int>(h % static_cast<std::uint32_t>(tableSize));
    }

    std::pair<int, int> FindCell(float x, float y) const {
        int gridX = static_cast<int>(std::floor(x / cellSize));
        int gridY = static_cast<int>(std::floor(y / cellSize));
        return {gridX, gridY};
    }

    void AddEntity(IDRegistry::GlobalID id, float x, float y) {
        std::pair<int, int> cellCoords = FindCell(x, y);
        int hash = GenerateHash(cellCoords.first, cellCoords.second);
        ids.push_back(id);
        cellX.push_back(cellCoords.first);
        cellY.push_back(cellCoords.second);

        next.push_back(head[hash]);
        head[hash] =  ids.size() - 1;
    }

    std::vector<IDRegistry::GlobalID> CheckInteractions(float x, float y) const {
        std::vector<IDRegistry::GlobalID> nearbyIDs;
        const auto [centerX, centerY] = FindCell(x, y);

        for (int offsetY = -1; offsetY <= 1; ++offsetY) {
            for (int offsetX = -1; offsetX <= 1; ++offsetX) {
                const int queryX = centerX + offsetX;
                const int queryY = centerY + offsetY;
                const int hash = GenerateHash(queryX, queryY);

                for (int entry = head[hash]; entry != -1; entry = next[entry]) {
                    // in the case of hash collisions, where entities in different cells end up in the same bucket
                    if (cellX[entry] == queryX && cellY[entry] == queryY) {
                        nearbyIDs.push_back(ids[entry]);
                    }
                }
            }
        }

        return nearbyIDs;
    }

    // Cleared & rebuilt every frame
    void Clear() {
        std::fill(head.begin(), head.end(), -1);
        ids.clear();
        cellX.clear();
        cellY.clear();
        next.clear();
    }

    int tableSize; // # of buckets
    int cellSize; // squares

    // assigned to each entity
    std::vector<int> ids;
    // Actual coordinates are required to reject unrelated hash collisions.
    std::vector<int> cellX;
    std::vector<int> cellY;
    // stores the id's of the first entity found in that bucket. len(head) = tableSize
    std::vector<int> head;
    // stores each entity's successor's (in its bucket) id. len(next) = num(particles)
    std::vector<int> next;
};
