#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"

#define CLAY_IMPLEMENTATION
#include "src/lib/clay.h"
#include "src/lib/clay_renderer_raylib.c"

#include "src/util.c"



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

    Shader imageShader = LoadShader(0, "./shaders/image.frag");

    Clay_Initialize(arena, (Clay_Dimensions){ init_window_width, init_window_height }, (Clay_ErrorHandler){ handle_clay_errors });

    Font fonts[] = {GetFontDefault()};

    Texture2D image = LoadTexture("./res/saft.png");
    float ratio = image.width / (float)image.height;
    float scale = 1.;

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();
        Vector2 mouse_scroll = GetMouseWheelMoveV();
        if (mouse_scroll.y < 0) { scale /= 1.1; }
        else if (mouse_scroll.y > 0) { scale *= 1.1; }
        scale = max(scale, 1);

        // do clay things
        Clay_SetLayoutDimensions((Clay_Dimensions){ GetScreenWidth(), GetScreenHeight() });
        Clay_SetPointerState((Clay_Vector2){ GetMouseX(), GetMouseY() }, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
        Clay_UpdateScrollContainers(true, raylib_vec2_to_clay_vec2(mouse_scroll), delta_time);

        Clay_BeginLayout();
        CLAY(CLAY_ID("ImageParent"), { 
            .layout = {
                .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER }
            },
            .backgroundColor = COLOR_SLATE
        }) {
            Clay_BoundingBox parent_bb = Clay_GetElementData(CLAY_ID("ImageParent")).boundingBox;
            printf("%f %f\n", parent_bb.height, parent_bb.width);
            float parent_apect_ratio = parent_bb.width / parent_bb.height;
            // Clay_Vector2 img_dims;
            // if (parent_apect_ratio > ratio) {  // parent is wider than image
            //     img_dims.y = parent_bb.height;
            //     img_dims.x = parent_bb.height * ratio;
            // }
            // else {
            //     img_dims.x = parent_bb.width;
            //     img_dims.y = parent_bb.width / ratio;
            // }
            // CLAY(CLAY_ID("ImageRect"), {
            //     .image = { (void*) &image },
            //     .aspectRatio = { ratio },
            //     .layout = {
            //         .sizing = { .width  = {.size = img_dims.x, .type = CLAY__SIZING_TYPE_FIXED},
            //                     .height = {.size = img_dims.y, .type = CLAY__SIZING_TYPE_FIXED}}
            //     },
            // }) { }
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
