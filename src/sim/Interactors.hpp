#pragma once

#include "CellPool.hpp"
#include "FoodPool.hpp"


// only for unique interactions

namespace interactions {
    void CellFood(CellPool& pool1, int index1, FoodPool& pool2, int index2);
}
