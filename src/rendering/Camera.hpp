#pragma once

#include <optional>
#include <raylib.h>

class PlayerCamera {
    private:
        Camera2D cam2d {};

        bool following { false };
        Vector2 targetPosition {};

        Vector2 zoomBounds { 0.2f, 5.0f };

        float speed { 3.0f };
        Vector2 speedBounds { 0.5, 8.0f };

    public:
        PlayerCamera(float screenHeight, float screenWidth);
        ~PlayerCamera();

        float zoomScale;
        
        void UpdateCamera(std::optional<Vector2> target = std::nullopt);

        void Zoom(float factor);
        void Follow(const Vector2& target);

        void ToggleFollowing(bool val);

        void Pan(float angle);

        float panSpeed() const;
        void SetPanSpeed(float value);

        void ResetTransform();

        Camera2D& data();
};
