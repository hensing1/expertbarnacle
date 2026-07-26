#include "raylib.h"
#include <stdio.h>

#include "src/colors.h"
#include "src/render.h"

const static int initWindowWidth = 800;
const static int initWindowHeight = 600;

int main() {
    InitWindow(initWindowWidth, initWindowHeight, "Expert image viewer deluxe edition +3");
    SetWindowMinSize(400, 300);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Shader imageShader = LoadShader(0, "src/shaders/image.frag");
    Texture2D image = LoadTexture("res/saft.png");
    SetTextureWrap(image, TEXTURE_WRAP_MIRROR_REPEAT);

    while (!WindowShouldClose()) {
        InputInfo inputs = {
            .mousePos = GetMousePosition(),
            .mouseScroll = GetMouseWheelMoveV(),
            .mouseLeft = IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
            .mouseRight = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)
        };

        Rectangle screenRect = {
            .x = 0, .y = 0, .width = GetScreenWidth(), .height = GetScreenHeight()
        };

        BeginDrawing();
        {
            DrawRectangleRec(screenRect, SLATE);
            renderImage(image, imageShader, screenRect, inputs);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
