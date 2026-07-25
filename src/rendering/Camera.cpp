#include <algorithm>
#include <raylib.h>
#include <raymath.h>

#include "Camera.hpp"
#include "sim_constants.hpp"

PlayerCamera::PlayerCamera(float screenHeight, float screenWidth) {
    cam2d.target = Vector2{settings::WORLD_WIDTH * 0.5f, settings::WORLD_HEIGHT * 0.5f};
    cam2d.offset = Vector2{screenWidth * 0.5f, screenHeight * 0.5f}; // center in middle of screen
    cam2d.rotation = 0.0f;
    cam2d.zoom = 1.0f;
}

PlayerCamera::~PlayerCamera() = default;

void PlayerCamera::zoom(float increment) {
    cam2d.zoom = std::clamp(cam2d.zoom + increment, zoomBounds.x, zoomBounds.y);
}

void PlayerCamera::follow(const Vector2& target) {
    cam2d.target = target;
}


void PlayerCamera::toggleFollowing() {
    following = !following;
}

void PlayerCamera::pan(float angle) {
    cam2d.rotation += angle;
}

void PlayerCamera::resetTransform() {
    cam2d.target = Vector2{settings::WORLD_WIDTH * 0.5f, settings::WORLD_HEIGHT * 0.5f};
    cam2d.zoom = 1.0f;
    following = false;
}

Camera2D& PlayerCamera::data() {
    return cam2d;
}