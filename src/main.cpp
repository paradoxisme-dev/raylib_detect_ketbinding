#include "raylib.h"
#include <emscripten.h>
#include <emscripten/val.h>
#include <string>
#include <iostream>

// Global state
std::string keyW = "W", keyA = "A", keyS = "S", keyD = "D";
std::string message = "Current Layout (not detected):";

// 1. Export C++ functions so JS can call them
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void UpdateKeys(const char* w, const char* a, const char* s, const char* d) {
        message = "Current Layout (detected):";
        keyW = w; keyA = a; keyS = s; keyD = d;
    }
}

// 2. Inject JS code to handle the promise and call the C++ function
EM_JS(void, FetchKeyboardLayout_JS, (), {
    if (navigator.keyboard && navigator.keyboard.getLayoutMap) {
        navigator.keyboard.getLayoutMap().then(layoutMap => {
            // Helper to get value or default
            const get = (code) => layoutMap.get(code) || code[3];
            
            // Call the C++ function we exported above
            _UpdateKeys(
                stringToNewUTF8(get("KeyW")),
                stringToNewUTF8(get("KeyA")),
                stringToNewUTF8(get("KeyS")),
                stringToNewUTF8(get("KeyD"))
            );
        });
    }
});

void UpdateDrawFrame() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // Define colors
    Color colorW = IsKeyDown(KEY_W) ? GREEN : MAROON;
    Color colorA = IsKeyDown(KEY_A) ? GREEN : MAROON;
    Color colorS = IsKeyDown(KEY_S) ? GREEN : MAROON;
    Color colorD = IsKeyDown(KEY_D) ? GREEN : MAROON;
    DrawText(message.c_str(), 100, 100, 20, DARKGRAY);
    DrawText(keyW.c_str(), 200, 150, 40, colorW);
    DrawText(keyA.c_str(), 150, 200, 40, colorA);
    DrawText(keyS.c_str(), 200, 200, 40, colorS);
    DrawText(keyD.c_str(), 250, 200, 40, colorD);
    EndDrawing();
}

int main() {
    InitWindow(720, 720, "Ergonomic Key Display");
    FetchKeyboardLayout_JS(); // Call the JS glue code
    
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    CloseWindow();
    return 0;
}