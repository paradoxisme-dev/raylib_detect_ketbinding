#include "raylib.h"
#include <emscripten.h>
#include <emscripten/val.h>
#include <string>
#include <cctype>

// Global state - store BOTH display char AND the keycode to check
int checkW = KEY_W, checkA = KEY_A, checkS = KEY_S, checkD = KEY_D;
std::string keyW = "W", keyA = "A", keyS = "S", keyD = "D";
std::string message = "Current Layout (not detected):";

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    void UpdateKeys(const char* w, const char* a, const char* s, const char* d) {
        message = "Current Layout (detected):";
        keyW = w; keyA = a; keyS = s; keyD = d;
        
        // Convert character to raylib keycode (uppercase ASCII value)
        checkW = std::toupper(static_cast<unsigned char>(w[0]));
        checkA = std::toupper(static_cast<unsigned char>(a[0]));
        checkS = std::toupper(static_cast<unsigned char>(s[0]));
        checkD = std::toupper(static_cast<unsigned char>(d[0]));
    }
}

EM_JS(void, FetchKeyboardLayout_JS, (), {
    if (navigator.keyboard && navigator.keyboard.getLayoutMap) {
        navigator.keyboard.getLayoutMap().then(layoutMap => {
            const get = (code) => layoutMap.get(code) || code[3];
            
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
    
    // Use the DYNAMICALLY determined key codes, not KEY_W/KEY_A/...
    Color colorW = IsKeyDown(checkW) ? GREEN : MAROON;
    Color colorA = IsKeyDown(checkA) ? GREEN : MAROON;
    Color colorS = IsKeyDown(checkS) ? GREEN : MAROON;
    Color colorD = IsKeyDown(checkD) ? GREEN : MAROON;
    
    DrawText(message.c_str(), 100, 100, 20, DARKGRAY);
    DrawText(keyW.c_str(), 200, 150, 40, colorW);
    DrawText(keyA.c_str(), 150, 200, 40, colorA);
    DrawText(keyS.c_str(), 200, 200, 40, colorS);
    DrawText(keyD.c_str(), 250, 200, 40, colorD);
    EndDrawing();
}

int main() {
    InitWindow(720, 720, "Ergonomic Key Display");
    FetchKeyboardLayout_JS();
    
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    CloseWindow();
    return 0;
}