#include <raylib.h>

int main()
{
    InitWindow(1000, 700, "Emergent World");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);
        DrawCircle(500, 350, 10.0F, SKYBLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}