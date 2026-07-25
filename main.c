#include "raylib.h"
#include <stdio.h>

#include "src/colors.h"
#include "src/util.c"

int main() {
    const int screenWidth  = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "guten Morgen");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Shader imageShader = LoadShader(0, "src/shaders/image.frag");
    Texture2D image = LoadTexture("res/saft.png");
    SetTextureWrap(image, TEXTURE_WRAP_MIRROR_REPEAT);

    float imageAspectRatio = (float)image.width / image.height;

    float scale = 1;

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        printf("%f %f\n", mousePos.x, mousePos.y);
        Vector2 mouse_scroll = GetMouseWheelMoveV();
        if (mouse_scroll.y < 0) { scale /= 1.15; }
        else if (mouse_scroll.y > 0) { scale *= 1.15; }
        scale = max(min(scale, 300), 1);
        Rectangle imageCrop;
        imageCrop.width = image.width / scale;
        imageCrop.height = image.height / scale;
        imageCrop.x = (image.width - imageCrop.width) / 2;
        imageCrop.y = (image.height - imageCrop.height) / 2;

        Vector2 screenDims = {GetScreenWidth(), GetScreenHeight()};
        float screenAspectRatio = screenDims.x / screenDims.y;
        Rectangle imageTarget;
        imageTarget.x = imageTarget.y = 0;

        if (screenAspectRatio > imageAspectRatio) {  // screen wider than image
            imageTarget.height = screenDims.y;
            imageTarget.width = imageTarget.height * imageAspectRatio;
            imageTarget.x = (screenDims.x - imageTarget.width) / 2;
        }
        else {
            imageTarget.width = screenDims.x;
            imageTarget.height = screenDims.x / imageAspectRatio;
            imageTarget.y = (screenDims.y - imageTarget.height) / 2;
        }

        BeginDrawing();
        {
            ClearBackground(SLATE);
            BeginShaderMode(imageShader);
            {
                // SetShaderValue(
                //     imageShader, picScaleLoc, &imgScale, SHADER_UNIFORM_FLOAT
                // );
                DrawTexturePro(
                    image,
                    imageCrop,
                    imageTarget,
                    (Vector2){0, 0},
                    0,
                    RED
                );
            }
            EndShaderMode();
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
