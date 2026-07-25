#include "Interactors.hpp"
#include <raymath.h>

#if 0
// Forward declare the utility so it is visible to the methods below.
// Actually defining it after the methods still works for templates, but
// the compiler needs to see the declaration before instantiation.
#endif

template <typename CircularPool>
void SimpleCircularBounce(CircularPool& pool1, int index1, CircularPool& pool2, int index2);

// Empty for now
Interactors::Interactors() {}
Interactors::~Interactors() {}

// Interactors

void Interactors::CellCell(CellPool& pool1, int index1, CellPool& pool2, int index2) {
    SimpleCircularBounce(pool1, index1, pool2, index2);
}

void Interactors::CellFood(CellPool& pool1, int index1, FoodPool& pool2, int index2) {
    // Implement damage
}

void Interactors::FoodFood(FoodPool& pool1, int index1, FoodPool& pool2, int index2) {
    SimpleCircularBounce(pool1, index1, pool2, index2);
}


// Utility

template <typename CircularPool>
void SimpleCircularBounce(CircularPool& pool1, int index1, CircularPool& pool2, int index2) {
    float radSum = pool1.radius[index1] + pool2.radius[index2];
    float dist = Vector2Distance(pool1.transform[index1].position, pool2.transform[index2].position);
    float overlap = radSum - dist;
    Vector2 normal = Vector2Normalize(Vector2Subtract(pool2.transform[index2].position, pool1.transform[index1].position));
    float push = overlap * 0.5f;
    pool1.transform[index1].position = Vector2Subtract(pool1.transform[index1].position, Vector2Scale(normal, push));
    pool2.transform[index2].position = Vector2Add(pool2.transform[index2].position, Vector2Scale(normal, push));

    Vector2 relativeVelocity = Vector2Subtract(pool2.transform[index2].velocity, pool1.transform[index1].velocity);
    float velAlongNormal = Vector2DotProduct(relativeVelocity, normal);

    // Impulse & Restitution
    if (velAlongNormal < 0) {
        float restitution = 0.85f; // Bounciness 
        float impulseMagnitude = -(1.0f + restitution) * velAlongNormal / 2.0f;
        Vector2 impulse = Vector2Scale(normal, impulseMagnitude);
        
        // Denominator expands with complex rotational mass distribution ^ 

        pool1.transform[index1].velocity = Vector2Subtract(pool1.transform[index1].velocity, impulse);
        pool2.transform[index2].velocity = Vector2Add(pool2.transform[index2].velocity, impulse);
    }
}