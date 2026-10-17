#include <raylib.h>

#define CLAY_IMPLEMENTATION
#include "src/lib/clay.h"
#define ARENA_IMPLEMENTATION
#include "src/lib/arena.h"

#include "src/renderer.h"

#include <stdio.h>
#include <stdint.h>

void handleClayErrors(Clay_ErrorData errors) {
    fprintf(stderr, "Clay Error: %s\n", errors.errorText.chars);
}

int main() {
    InitWindow(1000, 800, "hi");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    Texture2D yippieTexture = LoadTexture("./res/icons/ci--add-minus-square.png");

    uint64_t clayArenaSize = Clay_MinMemorySize();
    Clay_Arena clayArena = Clay_CreateArenaWithCapacityAndMemory(clayArenaSize, malloc(clayArenaSize));
    Clay_Initialize(
        clayArena,
        (Clay_Dimensions){ 1000, 800 },
        (Clay_ErrorHandler){ handleClayErrors }
    );
    while(!WindowShouldClose()) {
        Clay_BeginLayout();
        CLAY(CLAY_ID("global"), {
            .layout = {.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}},
        }) {
            CLAY(CLAY_ID("image"), {
                .layout = {.sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()}},
                .aspectRatio = {1.f},
                .image = { .imageData = (void*) &yippieTexture},
                .backgroundColor = {0, 0, 0, 127}
            });
        }
        Clay_RenderCommandArray renderCommands = Clay_EndLayout(GetFrameTime());

        BeginDrawing();
            ClearBackground(WHITE);
            Clay_Raylib_Render(renderCommands);
            // DrawTexturePro(yippieTexture, (Rectangle){0, 0, yippieTexture.width, yippieTexture.height},
            //     (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, (Vector2){0}, 0,
            //     (Color){0, 255, 0, 127});
        EndDrawing();
    }
    return 0;
}
