#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "raylib.h"

#define CLAY_IMPLEMENTATION
#include "lib/clay.h"
#include "lib/clay_renderer_raylib.c"


#define COLOR_SLATE (Clay_Color){43, 41, 51, 255}

#define CLAY_COLOR_TO_RAYLIB_COLOR(color)                                      \
    (Color) {                                                                  \
        .r = (unsigned char)roundf(color.r),                                   \
        .g = (unsigned char)roundf(color.g),                                   \
        .b = (unsigned char)roundf(color.b),                                   \
        .a = (unsigned char)roundf(color.a)                                    \
    }

static inline Clay_Vector2 raylib_vec2_to_clay_vec2(Vector2 vec2) { return (Clay_Vector2){vec2.x, vec2.y}; }


const static int init_window_width = 800;
const static int init_window_height = 600;


void handle_clay_errors(Clay_ErrorData error_data) {
    printf("oh no %s\n", error_data.errorText.chars);
}

int main() {
    uint64_t arena_size = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(arena_size, malloc(arena_size));

    InitWindow(init_window_width, init_window_height, "Expert image viewer deluxe edition +3");
    SetWindowMinSize(400, 300);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Clay_Initialize(arena, (Clay_Dimensions){ init_window_width, init_window_height }, (Clay_ErrorHandler){ handle_clay_errors });

    Font fonts[] = {GetFontDefault()};

    Texture2D image = LoadTexture("./res/saft.png");
    float ratio = image.width / (float)image.height;

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        // do clay things
        if (IsWindowResized()) {
            Clay_SetLayoutDimensions((Clay_Dimensions){ GetScreenWidth(), GetScreenHeight() });
        }
        Clay_SetPointerState((Clay_Vector2){ GetMouseX(), GetMouseY() }, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
        Clay_UpdateScrollContainers(true, raylib_vec2_to_clay_vec2(GetMouseWheelMoveV()), delta_time);

        Clay_BeginLayout();
        CLAY(CLAY_ID("ImageParent"), { 
            .layout = {
                .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER }
            },
            .backgroundColor = COLOR_SLATE
        }) {
            CLAY(CLAY_ID("ImageRect"), {
                .image = { (void*) &image },
                .aspectRatio = { ratio },
                .layout = {
                    .sizing = { .width = CLAY_SIZING_GROW() }
                },
            }) { }
        }
        Clay_RenderCommandArray render_commands = Clay_EndLayout(delta_time);

        // do rendering things
        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(render_commands, fonts);
        EndDrawing();
    }

    CloseWindow();

    free(arena.memory);
    return 0;
}
