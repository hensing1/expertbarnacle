#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

#include "raylib.h"

#define CLAY_IMPLEMENTATION
#include "src/lib/clay.h"
#include "src/lib/clay_renderer_raylib.c"

#include "src/format.h"
#include "src/io.h"
#include "src/render.h"
#include "src/state.h"
#include "src/ui.h"
#include "src/util.h"

const Vector2 initWinDims = {1200, 900};

Vector2 getInitWindowDimensions(Image);
void handleClayErrors(Clay_ErrorData errors);
bool isFile(char* path);
char* parseArgs(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    char* filePath = parseArgs(argc, argv);
    if (!filePath) {return -1;}

    initLocale();

    ApplicationState state = initAppState(filePath);
    state.infoScreenOpen = true;
    Image image = LoadImage(filePath);
    ImageInfo imageInfo = loadImageInfo(image, filePath);

    // initialize window and raylib
    Vector2 initWindowDims = getInitWindowDimensions(image);
    InitWindow(initWindowDims.x, initWindowDims.y, imageInfo.fileName);
    SetWindowMinSize(600, 450);
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    Shader imageShader = LoadShader(0, "src/shaders/image.frag");
    // int picDimLoc = GetShaderLocation(imageShader, "picSize");
    // SetShaderValue(imageShader, picDimLoc, &(Vector2) {image.width, image.height}, SHADER_UNIFORM_VEC2);

    Texture2D imageTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    GenTextureMipmaps(&imageTexture);
    SetTextureWrap(imageTexture, TEXTURE_WRAP_MIRROR_REPEAT);
    SetTextureFilter(imageTexture, TEXTURE_FILTER_TRILINEAR);
    
    Font fonts[] = {LoadFontEx("./res/fonts/InterDisplay-Regular.otf", 32, NULL, 250), GetFontDefault()};

    // initialize clay
    uint64_t arena_size = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(arena_size, malloc(arena_size));
    Clay_Initialize(
        arena,
        (Clay_Dimensions){ initWindowDims.x, initWindowDims.y },
        (Clay_ErrorHandler){ handleClayErrors }
    );
    InitOverlay();
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);
    Clay__debugViewWidth = 600;

    Clay_String* strings = makeApplicationStrings(LANG_DE, imageInfo);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_D)) {
            Clay_SetDebugModeEnabled(!Clay_IsDebugModeEnabled());
        }
        if (IsKeyPressed(KEY_I)) {
            state.infoScreenOpen ^= true;
        }
        InputInfo inputs = captureInputs();
        Clay_RenderCommandArray uiRenderCommands = createUI(state, inputs, strings);

        Rectangle imageRect = CLAY_RECTANGLE_TO_RAYLIB_RECTANGLE(
            Clay_GetElementData(Clay_GetElementId(CLAY_STRING("ImageContainer"))).boundingBox);

        BeginDrawing();
        {
            // DrawRectangleRec(imageRect, SLATE);
            Clay_Raylib_Render(uiRenderCommands, fonts);
            renderImage(imageTexture, imageShader, imageRect, inputs);
        }
        EndDrawing();
    }
    CloseWindow();
    freeImageInfo(imageInfo);
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


Vector2 getInitWindowDimensions(Image image) {
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

