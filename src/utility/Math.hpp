#pragma once

#include <vector>
#include <raylib.h>
#include <cmath>
#include <algorithm>
#include <limits>

namespace math {

    struct Point {
        float x;
        float y;
    };

    struct LineSegment {
        Point start;
        Point end;
    };    

    inline float DistanceSquared(float x1, float y1, float x2, float y2) {
        const float dx = x2 - x1;
        const float dy = y2 - y1;
        return dx * dx + dy * dy;
    }

    inline float Distance(float x1, float y1, float x2, float y2) {
        return std::sqrt(DistanceSquared(x1, y1, x2, y2));
    }

    inline float GetRandomFloat(float min, float max) {
        float scale = (float)GetRandomValue(0, 2147483647) / 2147483647.0f;
        return min + scale * (max - min);
    }

    inline void relu(std::vector<float>& layer) {
        for (auto& neuron : layer) {
            neuron = std::max(0.0f, neuron);
        }
    }

    inline void tanh(std::vector<float>& layer) {
        for (auto& neuron : layer) {
            neuron = std::tanh(neuron);
        }
    }

    inline bool IsInRange(float x1, float y1, float x2, float y2, float radius) {
        return Distance(x1, y1, x2, y2) <= radius;
    }

    inline std::pair<float, float> RaycastRectangleIntersection(float x1, float y1, float x2, float y2, float rectX, float rectY, float rectWidth, float rectHeight) {
        // Solve A + t(B - A) = C + u(D - C) for each rectangle edge.
        // Using the 2D cross product avoids dividing by dx_a, so vertical
        // segments work exactly like every other segment.
        const Point a{x1, y1};
        const Point r{x2 - x1, y2 - y1};
        const auto cross = [](Point lhs, Point rhs) {
            return lhs.x * rhs.y - lhs.y * rhs.x;
        };

        std::vector<LineSegment> rectangleSides = {
            {{rectX, rectY}, {rectX + rectWidth, rectY}}, // bottom side
            {{rectX + rectWidth, rectY}, {rectX + rectWidth, rectY + rectHeight}}, // right side
            {{rectX + rectWidth, rectY + rectHeight}, {(rectX), (rectY + (rectHeight))}}, // top side
            {{rectX, rectY + rectHeight}, {rectX, rectY}} // left side
        };

        constexpr float epsilon = 1e-6f;
        float nearestT = std::numeric_limits<float>::max();
        for (const auto& side : rectangleSides) {
            const Point c = side.start;
            const Point s{side.end.x - side.start.x, side.end.y - side.start.y};
            const Point q{c.x - a.x, c.y - a.y};
            const float denominator = cross(r, s);

            if (std::fabs(denominator) <= epsilon) {
                // Parallel/collinear edges do not have one unique crossing.
                // A collinear overlap is already represented by the endpoint
                // of the query segment, so handle that endpoint explicitly.
                if (std::fabs(cross(q, r)) <= epsilon &&
                    std::fabs(r.x) + std::fabs(r.y) > epsilon) {
                    const float rr = r.x * r.x + r.y * r.y;
                    const float t0 = ((c.x - a.x) * r.x + (c.y - a.y) * r.y) / rr;
                    const float t1 = ((side.end.x - a.x) * r.x + (side.end.y - a.y) * r.y) / rr;
                    const float candidate = std::max(0.0f, std::min(t0, t1));
                    if (candidate <= std::max(t0, t1) + epsilon && candidate <= 1.0f + epsilon)
                        nearestT = std::min(nearestT, candidate);
                }
                continue;
            }

            const float t = cross(q, s) / denominator;
            const float u = cross(q, r) / denominator;
            if (t >= -epsilon && t <= 1.0f + epsilon &&
                u >= -epsilon && u <= 1.0f + epsilon) {
                nearestT = std::min(nearestT, std::clamp(t, 0.0f, 1.0f));
            }
        }

        if (nearestT == std::numeric_limits<float>::max()) return {-1.0f, -1.0f};
        return {x1 + nearestT * r.x, y1 + nearestT * r.y};
    }

    inline std::pair<float, float> RaycastCircleIntersection(float x1, float y1, float x2, float y2, float circX, float circY, float radius) {
        const float dx = x2 - x1;
        const float dy = y2 - y1;
        const float offsetX = x1 - circX;
        const float offsetY = y1 - circY;
        const float a = dx * dx + dy * dy;
        const float b = 2.0f * (offsetX * dx + offsetY * dy);
        const float c = offsetX * offsetX + offsetY * offsetY - radius * radius;

        if (a == 0.0f) {
            return c <= 0.0f ? std::pair<float, float>{x1, y1}
                             : std::pair<float, float>{-1.0f, -1.0f};
        }

        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f) {
            return {-1.0f, -1.0f};
        } 

        const float sqrtDiscriminant = std::sqrt(discriminant);
        const float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
        const float t2 = (-b + sqrtDiscriminant) / (2.0f * a);
        float t = std::numeric_limits<float>::max();
        if (t1 >= 0.0f && t1 <= 1.0f) t = t1;
        if (t2 >= 0.0f && t2 <= 1.0f) t = std::min(t, t2);
        if (t == std::numeric_limits<float>::max()) return {-1.0f, -1.0f};
        return {x1 + t * dx, y1 + t * dy};
    }

    inline float SignedAngleBetweenVectors(Vector2 a, Vector2 b) {
        // angle to take a to b

        return std::atan2(
            a.x * b.y - a.y * b.x,
            a.x * b.x + a.y * b.y
        );
    }
}
