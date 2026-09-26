#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

#include "raylib.h"

#include "src/format.h"
#include "src/image.h"
#include "src/io.h"
#include "src/render.h"
#include "src/state.h"
#include "src/ui.h"
#include "src/util.h"

#define ARENA_IMPLEMENTATION
#include "src/lib/arena.h"

#define CLAY_IMPLEMENTATION
#include "src/lib/clay.h"
#include "src/lib/clay_renderer_raylib.c"

const Vector2 initWinDims = {1200, 900};

Vector2 getInitWindowDimensions(char* imagePath);
void handleClayErrors(Clay_ErrorData errors);
char* parseArgs(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    char* filePath = parseArgs(argc, argv);
    if (!filePath) {return -1;}

    // initialize window and raylib
    Vector2 initWindowDims = getInitWindowDimensions(filePath);
    InitWindow(initWindowDims.x, initWindowDims.y, filePath);
    SetWindowMinSize(600, 450);
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Shader imageShader = LoadShader(0, "src/shaders/image.frag");
    // int picDimLoc = GetShaderLocation(imageShader, "picSize");
    // SetShaderValue(imageShader, picDimLoc, &(Vector2) {image.width, image.height}, SHADER_UNIFORM_VEC2);
    
    char* fontFile = "./res/fonts/adwaita-sans/static/adwaita-sans-latin-500-normal.ttf";
    Font fonts[] = {LoadFontEx(fontFile, 28, NULL, 250), LoadFontEx(fontFile, 32, NULL, 250)};

    // initialize clay
    uint64_t clayArenaSize = Clay_MinMemorySize();
    Clay_Arena clayArena = Clay_CreateArenaWithCapacityAndMemory(clayArenaSize, malloc(clayArenaSize));
    Clay_Initialize(
        clayArena,
        (Clay_Dimensions){ initWindowDims.x, initWindowDims.y },
        (Clay_ErrorHandler){ handleClayErrors }
    );
    InitOverlay();
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);
    Clay__debugViewWidth = 600;

    initLocale();
    ApplicationState state = initAppState(filePath);
    ImageData image = loadImage(filePath); 

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_D)) {
            Clay_SetDebugModeEnabled(!Clay_IsDebugModeEnabled());
        }
        if (IsKeyPressed(KEY_I)) {
            state.infoScreenOpen ^= true;
        }
        InputInfo inputs = captureInputs();
        Clay_RenderCommandArray uiRenderCommands = createUI(state, inputs, image.strings);

        Rectangle imageRect = CLAY_RECTANGLE_TO_RAYLIB_RECTANGLE(
            Clay_GetElementData(Clay_GetElementId(CLAY_STRING("ImageContainer"))).boundingBox);

        BeginDrawing();
        {
            // DrawRectangleRec(imageRect, SLATE);
            Clay_Raylib_Render(uiRenderCommands, fonts);
            renderImage(image.texture, imageShader, imageRect, inputs);
        }
        EndDrawing();
    }
    arena_free(&image.arena);
    CloseWindow();
    return 0;
}

char* parseArgs(int argc, char* argv[]) {
    if (argc == 1) {
        return "res/Kugelquiek.jpg";
    }
    if (!isFile(argv[1])) {
        fprintf(stderr, "ERROR: Not a file: %s\n", argv[1]);
        return NULL;
    }
    return argv[1];
}


Vector2 getInitWindowDimensions(char* imagePath) {
    Image image = LoadImage(imagePath); // TODO: find better way to get image dims
    if (!IsImageValid(image)) {
        return initWinDims;
    }
    float imageAspectRatio = (float)image.width / image.height;
    float width, height;
    if (imageAspectRatio > 1.333) {
        width = min(image.width, initWinDims.x);
        height = max(width / imageAspectRatio, 300);
    }
    else {
        height = min(image.height, initWinDims.y);
        width = max(height * imageAspectRatio, 400);
    }
    return (Vector2){width, height};
}

void handleClayErrors(Clay_ErrorData errors) {
    fprintf(stderr, "Clay Error: %s\n", errors.errorText.chars);
}

