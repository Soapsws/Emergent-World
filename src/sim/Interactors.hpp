#include "CellPool.hpp"
#include "FoodPool.hpp"

struct Interactors {

    Interactors();
    ~Interactors();

    // Circular Interactors

    // (LEARNING) Pass CellPool by reference to prevent auto copying

    void CellCell(CellPool& pool1, int index1, CellPool& pool2, int index2);
    void CellFood(CellPool& pool1, int index1, FoodPool& pool2, int index2);
    void FoodFood(FoodPool& pool1, int index1, FoodPool& pool2, int index2);

};