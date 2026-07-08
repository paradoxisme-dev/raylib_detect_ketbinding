#include "raylib.h"
#include <string>
#include <iostream>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include <emscripten/val.h>
#endif

// Global state to store our key labels
std::string keyW = "W";
std::string keyA = "A";
std::string keyS = "S";
std::string keyD = "D";

// Helper to fetch the map from the browser
void FetchKeyboardLayout() {
#ifdef __EMSCRIPTEN__
    // Keep this best-effort only: any failure should not block rendering.
    try {
    auto navigator = emscripten::val::global("navigator");
    auto keyboard = navigator["keyboard"];

    if (keyboard.isUndefined() || keyboard.isNull()) {
        std::cout << "Keyboard Layout API not available; using defaults." << std::endl;
        return;
    }

    auto getLayoutMap = keyboard["getLayoutMap"];
    if (getLayoutMap.isUndefined() || getLayoutMap.isNull()) {
        std::cout << "getLayoutMap() not available; using defaults." << std::endl;
        return;
    }
    
    // Promise callbacks must be JS functions; do not attach invalid callbacks from C++.
    // We only probe availability here to avoid JS exceptions that can abort the app.
    keyboard.call<emscripten::val>("getLayoutMap");
    } catch (...) {
        std::cout << "Keyboard layout probe failed; using defaults." << std::endl;
    }
#endif
}

void UpdateDrawFrame() {
    // This function is called every frame
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Displaying the mapped keys
    DrawText("Current WASD layout on your keyboard:", 100, 100, 20, DARKGRAY);
    
    // Drawing them in a cluster
    DrawText(keyW.c_str(), 200, 150, 40, MAROON);
    DrawText(keyA.c_str(), 150, 200, 40, MAROON);
    DrawText(keyS.c_str(), 200, 200, 40, MAROON);
    DrawText(keyD.c_str(), 250, 200, 40, MAROON);

    EndDrawing();
}

int main() {
    InitWindow(720, 720, "Ergonomic Key Display");
    FetchKeyboardLayout();

    #ifdef __EMSCRIPTEN__
        // emscripten_set_main_loop(function, fps, simulate_infinite_loop)
        emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    #else
        SetTargetFPS(60);
        while (!WindowShouldClose()) {
            UpdateDrawFrame();
        }
    #endif

    CloseWindow();
    return 0;
}