#include <algorithm>
#include <raylib.h>
#include <raymath.h>

#include "Camera.hpp"
#include "sim_constants.hpp"

PlayerCamera::PlayerCamera(float screenWidth, float screenHeight) {
    cam2d.target = Vector2{settings::WORLD_WIDTH * 0.5f, settings::WORLD_HEIGHT * 0.5f};
    cam2d.offset = Vector2{screenWidth * 0.5f, screenHeight * 0.5f}; // center in middle of screen
    cam2d.rotation = 0.0f; // unnecessary?
    cam2d.zoom = 1.0f;
    zoomScale = 0.8;
}

PlayerCamera::~PlayerCamera() = default;

void PlayerCamera::UpdateCamera(std::optional<Vector2> target) {
    if (following && target.has_value()) {
        Follow(target.value());
    }
}

void PlayerCamera::Zoom(float factor) {
    cam2d.zoom = std::clamp(cam2d.zoom * factor, zoomBounds.x, zoomBounds.y);
}

void PlayerCamera::Follow(const Vector2& target) {
    cam2d.target = target;
}


void PlayerCamera::ToggleFollowing(bool val) {
    following = val;
}

// Raylib uses degrees; C++ uses radians
void PlayerCamera::Pan(float angle) {
    cam2d.target = Vector2Add(cam2d.target, Vector2{speed * cosf(angle * DEG2RAD), speed * sinf(angle * DEG2RAD)});
}

float PlayerCamera::panSpeed() const {
    return speed;
}

void PlayerCamera::SetPanSpeed(float value) {
    speed = std::clamp(value, speedBounds.x, speedBounds.y);
}

void PlayerCamera::ResetTransform() {
    cam2d.target = Vector2{settings::WORLD_WIDTH * 0.5f, settings::WORLD_HEIGHT * 0.5f};
    cam2d.zoom = 1.0f;
    following = false;
}

Camera2D& PlayerCamera::data() {
    return cam2d;
}
