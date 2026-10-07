#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

#include "raylib.h"

#include "src/fonts.h"
#include "src/format.h"
#include "src/image.h"
#include "src/io.h"
#include "src/renderer.h"
#include "src/state.h"
#include "src/ui.h"
#include "src/util.h"

#define ARENA_IMPLEMENTATION
#include "src/lib/arena.h"

#define CLAY_IMPLEMENTATION
#include "src/lib/clay.h"

const Vector2 initWinDims = {1200, 900};

Vector2 getInitWindowDimensions(char* imagePath);
void handleClayErrors(Clay_ErrorData errors);
char* parseArgs(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    char* filePath = parseArgs(argc, argv);
    if (!filePath) {return -1;}

    // initialize window and raylib
    Vector2 initWindowDims = getInitWindowDimensions(filePath);
    InitWindow(initWindowDims.x, initWindowDims.y, "Bildfresser 3000");
    SetWindowMinSize(600, 450);
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    initImageShader();

    // initialize clay
    uint64_t clayArenaSize = Clay_MinMemorySize();
    Clay_Arena clayArena = Clay_CreateArenaWithCapacityAndMemory(clayArenaSize, malloc(clayArenaSize));
    Clay_Initialize(
        clayArena,
        (Clay_Dimensions){ initWindowDims.x, initWindowDims.y },
        (Clay_ErrorHandler){ handleClayErrors }
    );
    Clay__debugViewWidth = 600;

    char* fontFile = "./res/fonts/adwaita-sans/static/adwaita-sans-latin-400-normal.ttf";
    // char* fontFile = "./res/fonts/Libron/Libron-Regular.ttf";
    // char* fontFile = "./res/fonts/ia-writer-quattro/ia-writer-quattro-latin-400-normal.ttf";
    initFont(fontFile);
    Clay_Raylib_Initialize(NULL);

    initLocale();
    ApplicationState state = initAppState(filePath);
    SetWindowTitle(state.currentImage.metadata.fileName);

    while (!WindowShouldClose()) {
        InputInfo inputs = captureInputs();
        Arena frameArena = {};
        Clay_RenderCommandArray uiRenderCommands = createUI(state, inputs, &frameArena);

        BeginDrawing();
        {
            Clay_Raylib_Render(uiRenderCommands);
        }
        EndDrawing();

        updateState(&state, inputs);
        arena_free(&frameArena);
    }

    freeImage(state.currentImage);
    CloseWindow();
    Clay_Raylib_Close();
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
        exit(-1);
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
    UnloadImage(image);
    return (Vector2){width, height};
}

void handleClayErrors(Clay_ErrorData errors) {
    fprintf(stderr, "Clay Error: %s\n", errors.errorText.chars);
}

