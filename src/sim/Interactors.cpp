#include "Interactors.hpp"
#include "Physics.hpp"

namespace interactions {

void CellFood(CellPool& pool1, int index1, FoodPool& pool2, int index2) {
    physics::CircularBounce(pool1, index1, pool2, index2, 0.1f);
    pool2.health[index2] -= pool1.dps[index1] * GetFrameTime();
}

} 
