#include "raylib.h"
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif


void UpdateDrawFrame(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first raylib game!", 190, 200, 20, LIGHTGRAY);
    EndDrawing();
}

int main(void) {
    InitWindow(720, 720, "Raylib Web Game");

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
