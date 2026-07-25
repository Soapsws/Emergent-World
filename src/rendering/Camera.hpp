#pragma once

#include <raylib.h>

class PlayerCamera {
    private:
        Camera2D cam2d {};
        bool following { false };
        Vector2 zoomBounds { 0.2f, 5.0f };

    public:
        PlayerCamera(float screenHeight, float screenWidth);
        ~PlayerCamera();

        void zoom(float increment);
        void follow(const Vector2& target);

        void toggleFollowing();

        void pan(float angle);

        void resetTransform();

        Camera2D& data();
};