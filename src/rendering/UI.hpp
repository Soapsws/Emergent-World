#pragma once

#include "Camera.hpp"

struct UI {

    UI(bool useDarkTheme, PlayerCamera& camera, int& maxCells, int& maxFood);
    ~UI();

    /*
    prevention of double-destruction bug:
        if a copy of UI is made, the copy will destroy the underlying rlImGui library
        when it goes out of scope, incapacitating all running versions of UI.
        this makes the program unable to compile if it detects a copy of UI.
    */
    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;

    void Begin();
    void End();

    void Draw();

private:
    PlayerCamera& camera;
    int& maxCells;
    int& maxFood;

};
