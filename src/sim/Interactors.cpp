#include "Interactors.hpp"
#include <raymath.h>
#include <algorithm>

#if 0
// Forward declare the utility so it is visible to the methods below.
// Actually defining it after the methods still works for templates, but
// the compiler needs to see the declaration before instantiation.
#endif

template <typename CircularPool1, typename CircularPool2>
void SimpleCircularBounce(CircularPool1& pool1, int index1, CircularPool2& pool2, int index2, float restitution);

template <typename CircularPool, typename RectangularPool>
void CircularImmovableRectangleBounce(
    CircularPool& pool1,
    int index1,
    RectangularPool& pool2,
    int index2,
    float restitution
);

// Empty for now
Interactors::Interactors() {}
Interactors::~Interactors() {}

// Interactors

void Interactors::CellCell(CellPool& pool1, int index1, CellPool& pool2, int index2) {
    SimpleCircularBounce(pool1, index1, pool2, index2, 0.85f);
}

void Interactors::CellFood(CellPool& pool1, int index1, FoodPool& pool2, int index2) {
    SimpleCircularBounce(pool1, index1, pool2, index2, 0.1f);
    pool2.health[index2] -= pool1.dps[index1] * GetFrameTime();
}

void Interactors::FoodFood(FoodPool& pool1, int index1, FoodPool& pool2, int index2) {
    SimpleCircularBounce(pool1, index1, pool2, index2, 0.6f);
}


void Interactors::CellWall(CellPool& pool1, int index1, Walls& pool2, int index2) {
    CircularImmovableRectangleBounce(pool1, index1, pool2, index2, 0.6f);
}

void Interactors::FoodWall(FoodPool& pool1, int index1, Walls& pool2, int index2) {
    CircularImmovableRectangleBounce(pool1, index1, pool2, index2, 0.6f);
}


// Utility

template <typename CircularPool1, typename CircularPool2>
void SimpleCircularBounce(CircularPool1& pool1, int index1, CircularPool2& pool2, int index2, float restitution) {
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
        float impulseMagnitude = -(1.0f + restitution) * velAlongNormal / 2.0f;
        Vector2 impulse = Vector2Scale(normal, impulseMagnitude);
        
        // Denominator expands with complex rotational mass distribution ^ 

        pool1.transform[index1].velocity = Vector2Subtract(pool1.transform[index1].velocity, impulse);
        pool2.transform[index2].velocity = Vector2Add(pool2.transform[index2].velocity, impulse);
    }
}

template <typename CircularPool, typename RectangularPool>
void CircularImmovableRectangleBounce(
    CircularPool& pool1,
    int index1,
    RectangularPool& pool2,
    int index2,
    float restitution
) {
    Vector2& center = pool1.transform[index1].position;
    Vector2& velocity = pool1.transform[index1].velocity;
    const float radius = pool1.radius[index1];
    const Rectangle& rectangle = pool2.walls[index2];

    const float closestX = std::clamp(center.x, rectangle.x, rectangle.x + rectangle.width);
    const float closestY = std::clamp(center.y, rectangle.y, rectangle.y + rectangle.height);
    const Vector2 closestPoint{ closestX, closestY };
    Vector2 normal = Vector2Subtract(center, closestPoint);
    const float distance = Vector2Length(normal);

    if (distance > 0.0f) {
        normal = Vector2Scale(normal, 1.0f / distance);
        center = Vector2Add(center, Vector2Scale(normal, radius - distance));
    } else {
        const float left = center.x - rectangle.x;
        const float right = rectangle.x + rectangle.width - center.x;
        const float top = center.y - rectangle.y;
        const float bottom = rectangle.y + rectangle.height - center.y;
        const float nearest = std::min({ left, right, top, bottom });

        if (nearest == left) {
            normal = Vector2{ -1.0f, 0.0f };
            center.x = rectangle.x - radius;
        } else if (nearest == right) {
            normal = Vector2{ 1.0f, 0.0f };
            center.x = rectangle.x + rectangle.width + radius;
        } else if (nearest == top) {
            normal = Vector2{ 0.0f, -1.0f };
            center.y = rectangle.y - radius;
        } else {
            normal = Vector2{ 0.0f, 1.0f };
            center.y = rectangle.y + rectangle.height + radius;
        }
    }

    const float velocityIntoWall = Vector2DotProduct(velocity, normal);
    if (velocityIntoWall < 0.0f) {
        velocity = Vector2Subtract(
            velocity,
            Vector2Scale(normal, (1.0f + restitution) * velocityIntoWall)
        );
    }
}
