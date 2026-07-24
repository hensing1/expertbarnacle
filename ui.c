#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "raylib.h"

#define CLAY_IMPLEMENTATION
#include "lib/clay.h"

const static int init_window_width = 800;
const static int init_window_height = 600;

#define CLAY_COLOR_TO_RAYLIB_COLOR(color)                                      \
  (Color) {                                                                    \
    .r = (unsigned char)roundf(color.r),                                       \
    .g = (unsigned char)roundf(color.g),                                       \
    .b = (unsigned char)roundf(color.b),                                       \
    .a = (unsigned char)roundf(color.a)                                        \
  }

static inline Clay_Vector2 raylib_vec2_to_clay_vec2(Vector2 vec2) { return (Clay_Vector2){vec2.x, vec2.y}; }

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

  while (!WindowShouldClose()) {
    float delta_time = GetFrameTime();

    // do clay things
    if (IsWindowResized()) {
      Clay_SetLayoutDimensions((Clay_Dimensions){ GetScreenWidth(), GetScreenHeight() });
    }
    Clay_SetPointerState((Clay_Vector2){ GetMouseX(), GetMouseY() }, IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
    Clay_UpdateScrollContainers(true, raylib_vec2_to_clay_vec2(GetMouseWheelMoveV()), delta_time);

    Clay_BeginLayout();
    Clay_RenderCommandArray render_commands = Clay_EndLayout(delta_time);

    // do rendering things
    BeginDrawing();
    for (int i = 0; i < render_commands.length; i++) {
      Clay_RenderCommand* render_command = &render_commands.internalArray[i];
      Clay_BoundingBox bb = render_command->boundingBox;

      switch (render_command->commandType) {
        case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:
          DrawRectangle(bb.x, bb.y, bb.width, bb.height, CLAY_COLOR_TO_RAYLIB_COLOR(render_command->renderData.rectangle.backgroundColor));
          break;
        default:
          printf("some unsupported rendering is requested\n");
      }
    }
    EndDrawing();
  }

  CloseWindow();

  free(arena.memory);
  return 0;
}
