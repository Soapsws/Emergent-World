#pragma once

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

        void zoom(float factor);
        void follow(const Vector2& target);

        void toggleFollowing();

        void pan(float angle);

        float panSpeed() const;
        void setPanSpeed(float value);

        void resetTransform();

        Camera2D& data();
};
