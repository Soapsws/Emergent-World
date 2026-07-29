#pragma once

#include <algorithm>
#include <raymath.h>

// generic collisions

namespace physics {

template <typename CircularPool1, typename CircularPool2>
void CircularBounce(CircularPool1& pool1, int index1, CircularPool2& pool2,
                    int index2, float restitution) {
    const float radiusSum = pool1.radius[index1] + pool2.radius[index2];
    const float distance = Vector2Distance(
        pool1.transform[index1].position,
        pool2.transform[index2].position
    );
    const float overlap = radiusSum - distance;
    Vector2 normal = Vector2Normalize(Vector2Subtract(
        pool2.transform[index2].position,
        pool1.transform[index1].position
    ));

    const float push = overlap * 0.5f;
    pool1.transform[index1].position = Vector2Subtract(
        pool1.transform[index1].position,
        Vector2Scale(normal, push)
    );
    pool2.transform[index2].position = Vector2Add(
        pool2.transform[index2].position,
        Vector2Scale(normal, push)
    );

    const Vector2 relativeVelocity = Vector2Subtract(
        pool2.transform[index2].velocity,
        pool1.transform[index1].velocity
    );
    const float velocityAlongNormal = Vector2DotProduct(relativeVelocity, normal);

    if (velocityAlongNormal < 0.0f) {
        const float impulseMagnitude =
            -(1.0f + restitution) * velocityAlongNormal * 0.5f;
        const Vector2 impulse = Vector2Scale(normal, impulseMagnitude);

        pool1.transform[index1].velocity = Vector2Subtract(
            pool1.transform[index1].velocity, impulse
        );
        pool2.transform[index2].velocity = Vector2Add(
            pool2.transform[index2].velocity, impulse
        );
    }
}

template <typename CircularPool1, typename CircularPool2>
void CircularImmovableCircularBounce(CircularPool1& pool1, int index1,
                                        CircularPool2& pool2, int index2, float restitution) {
    const float radiusSum = pool1.radius[index1] + pool2.radius[index2];
    const float distance = Vector2Distance(
        pool1.transform[index1].position,
        pool2.transform[index2].position
    );
    const float overlap = radiusSum - distance;
    Vector2 normal = Vector2Normalize(Vector2Subtract(
        pool2.transform[index2].position,
        pool1.transform[index1].position
    ));

    const float push = overlap * 0.5f;
    pool1.transform[index1].position = Vector2Subtract(
        pool1.transform[index1].position,
        Vector2Scale(normal, push)
    );
    
    const Vector2 relativeVelocity = Vector2Subtract(
        pool2.transform[index2].velocity,
        pool1.transform[index1].velocity
    );
    const float velocityAlongNormal = Vector2DotProduct(relativeVelocity, normal);

    if (velocityAlongNormal < 0.0f) {
        const float impulseMagnitude =
            -(1.0f + restitution) * velocityAlongNormal * 0.5f;
        const Vector2 impulse = Vector2Scale(normal, impulseMagnitude);

        pool1.transform[index1].velocity = Vector2Subtract(
            pool1.transform[index1].velocity, impulse
        );
    }                                                           
}

template <typename CircularPool, typename RectangularPool>
void CircularImmovableRectangularBounce(CircularPool& pool1, int index1,
                                      RectangularPool& pool2, int index2,
                                      float restitution) {
    Vector2& center = pool1.transform[index1].position;
    Vector2& velocity = pool1.transform[index1].velocity;
    const float radius = pool1.radius[index1];
    const Rectangle& rectangle = pool2.walls[index2];

    const float closestX = std::clamp(center.x, rectangle.x, rectangle.x + rectangle.width);
    const float closestY = std::clamp(center.y, rectangle.y, rectangle.y + rectangle.height);
    Vector2 normal = Vector2Subtract(center, Vector2{closestX, closestY});
    const float distance = Vector2Length(normal);

    if (distance > 0.0f) {
        normal = Vector2Scale(normal, 1.0f / distance);
        center = Vector2Add(center, Vector2Scale(normal, radius - distance));
    } else {
        const float left = center.x - rectangle.x;
        const float right = rectangle.x + rectangle.width - center.x;
        const float top = center.y - rectangle.y;
        const float bottom = rectangle.y + rectangle.height - center.y;
        const float nearest = std::min({left, right, top, bottom});

        if (nearest == left) {
            normal = {-1.0f, 0.0f};
            center.x = rectangle.x - radius;
        } else if (nearest == right) {
            normal = {1.0f, 0.0f};
            center.x = rectangle.x + rectangle.width + radius;
        } else if (nearest == top) {
            normal = {0.0f, -1.0f};
            center.y = rectangle.y - radius;
        } else {
            normal = {0.0f, 1.0f};
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

} 
