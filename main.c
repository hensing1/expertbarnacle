#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

#include "raylib.h"

#define CLAY_IMPLEMENTATION
#include "src/lib/clay.h"
#include "src/lib/clay_renderer_raylib.c"

#include "src/colors.h"
#include "src/io.h"
#include "src/render.h"
#include "src/ui.h"
#include "src/util.h"

Vector2 getInitWindowDimensions(Image);
void handleClayErrors(Clay_ErrorData errors);
bool isFile(char* path);

int main(int argc, char* argv[]) {
    char* filePath;
    if (argc > 1) {
        if (!isFile(argv[1])) {
            fprintf(stderr, "ERROR: Not a file: %s\n", argv[1]);
            return -1;
        }
        filePath = argv[1];
    }
    else {
        filePath = "res/Kugelquiek.jpg";
    }

    const char* fileName = GetFileName(filePath);
    const char* fileDir = GetDirectoryPath(filePath);
    FilePathList allImgFiles = getImageFiles(fileDir);
    for (int i = 0; i < allImgFiles.count; i++) {
        printf("%s\n", allImgFiles.paths[i]);
    }
    UnloadDirectoryFiles(allImgFiles);

    Image image = LoadImage(filePath);
    // initialize window and raylib
    Vector2 initWindowDims = getInitWindowDimensions(image);
    InitWindow(initWindowDims.x, initWindowDims.y, fileName);
    SetWindowMinSize(400, 300);
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
    
    Font fonts[] = {LoadFont("./res/fonts/adwaita-sans/static/adwaita-sans-latin-500-normal.ttf"), GetFontDefault()};

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

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_D)) {
            Clay_SetDebugModeEnabled(!Clay_IsDebugModeEnabled());
        }
        InputInfo inputs = captureInputs();
        Clay_RenderCommandArray uiRenderCommands = createUI(inputs);

        Rectangle imageRect = CLAY_RECTANGLE_TO_RAYLIB_RECTANGLE(
            Clay_GetElementData(Clay_GetElementId(CLAY_STRING("ImageContainer"))).boundingBox);

        BeginDrawing();
        {
            DrawRectangleRec(imageRect, SLATE);
            Clay_Raylib_Render(uiRenderCommands, fonts);
            renderImage(imageTexture, imageShader, imageRect, inputs);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}


Vector2 getInitWindowDimensions(Image image) {
    if (!IsImageValid(image)) {
        return (Vector2){800, 600};
    }
    float imageAspectRatio = (float)image.width / image.height;
    float width, height;
    if (imageAspectRatio > 1.333) {
        width = min(image.width, 800);
        height = max(width / imageAspectRatio, 300);
    }
    else {
        height = min(image.height, 600);
        width = max(height * imageAspectRatio, 400);
    }
    return (Vector2){width, height};
}

void handleClayErrors(Clay_ErrorData errors) {
    fprintf(stderr, "Clay Error: %s\n", errors.errorText.chars);
}

bool isFile(char* file) {
    struct stat s;
    return stat(file, &s) == 0 && (s.st_mode & S_IFREG);
}
