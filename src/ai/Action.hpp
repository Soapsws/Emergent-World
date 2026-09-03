#pragma once

#include <raylib.h>
#include <raymath.h>

#include "sim_constants.hpp"
#include "EntityTypes.hpp"
#include "TransformAccess.hpp"

struct Action {

    // thrust, alpha bounded between -1 and 1
    float thrust;
    float alpha;

    Action(float thrust, float alpha);

    template <typename StatefulEntityPool>
    void ApplyOnEntity(StatefulEntityPool& entityPool, int index) const;
};

template <typename StatefulEntityPool>
void Action::ApplyOnEntity(StatefulEntityPool& entityPool, int index) const {
    const float dt = GetFrameTime();
    entity::Transform& transform = TransformAt(entityPool, index);
    Vector2& facing = [&]() -> Vector2& {
        if constexpr (requires { entityPool.state[index].facing; })
            return entityPool.state[index].facing;
        else
            return TransformAt(entityPool, index).velocity;
    }();

    constexpr float thrustAcceleration = 10.0f;
    transform.velocity = Vector2Add(
        transform.velocity,
        Vector2Scale(facing, thrust * thrustAcceleration * dt)
    );

    constexpr float angularAcceleration = 8.0f;
    transform.angularVelocity += alpha * angularAcceleration * dt;
    if constexpr (requires { entityPool.state[index].facing; }) {
        const float facingAngle = std::atan2(facing.y, facing.x)
            + transform.angularVelocity * dt;
        facing = {std::cos(facingAngle), std::sin(facingAngle)};
    }

    const float retention = std::clamp(1.0f - entityPool.drag[index], 0.0f, 1.0f);
    transform.velocity = Vector2Scale(transform.velocity, std::pow(retention, dt * 60.0f));
    transform.position = Vector2Add(transform.position, transform.velocity);

    if (transform.position.x < 0.0f) transform.position.x = settings::WORLD_WIDTH;
    else if (transform.position.x > settings::WORLD_WIDTH) transform.position.x = 0.0f;
    if (transform.position.y < 0.0f) transform.position.y = settings::WORLD_HEIGHT;
    else if (transform.position.y > settings::WORLD_HEIGHT) transform.position.y = 0.0f;
}
