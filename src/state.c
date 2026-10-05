#include "state.h"

#include <math.h>
#include <raylib.h>

#include "format.h"
#include "image.h"
#include "io.h"
#include "util.h"

ApplicationState initAppState(char* imagePath) {
    const char* fileDir = GetDirectoryPath(imagePath);
    int monitor = GetCurrentMonitor();
    ApplicationState s = {};

    s.prevWindowDims = (Vector2) {GetMonitorWidth(monitor), GetMonitorHeight(monitor)};
    s.timeSinceMouseMovement = 0.f;

    s.pointerDraggingState = POINTER_DEFAULT;
    s.infoScreenOpen = false;
    s.infoScreenWidth = 400;

    s.imgFilesInDir = getImagePaths(fileDir);
    s.currentImgIndex = findInImagePaths(s.imgFilesInDir, imagePath);
    s.currentImage = loadImage(imagePath);

    s.imageRenderParams = (RenderParams_MainImage){
        .zoom = 1,
        .zoomSpeed = 0,
        .zoomCenter = {},
        .isDragging = false,
        .imageCrop = (Rectangle) {
            .width = s.currentImage.texture.width,
            .height = s.currentImage.texture.height
        },
        .imageTarget = {}
    };

    return s;
}

static inline bool isButtonPressed(InputInfo inputs, const char* elementName) {
    return inputs.mouseLeftPressed && Clay_PointerOver(Clay_GetElementId(mkClayString(elementName)));
}

static inline bool isButtonHeld(InputInfo inputs, const char* elementName) {
    return inputs.mouseLeftDown && Clay_PointerOver(Clay_GetElementId(mkClayString(elementName)));
}

static inline bool isButtonHovered(InputInfo inputs, const char* elementName) {
    return !inputs.mouseLeftDown && Clay_PointerOver(Clay_GetElementId(mkClayString(elementName)));
}

static const float zoomSpeedBoost = 0.02f;
static const float zoomSpeedDecay = 0.85f;

static void updateZoom(RenderParams_MainImage* params, InputInfo inputs, Clay_BoundingBox viewport) {
    if (fabsf(params->zoom - 1) <= 0.01 && params->zoomSpeed != 0) {  // snap to 1x zoom
        params->zoom = 1;
        params->zoomSpeed = 0;
        return;
    }

    params->zoomSpeed *= powf(zoomSpeedDecay, 60.f * GetFrameTime());

    if (CheckCollisionPointRec(inputs.mousePos, TO_RAYLIB(viewport))) {
        if (sgn(params->zoomSpeed) == -sgn(inputs.mouseScroll.y)) {
            params->zoomSpeed = 0;
        }
        else {
            params->zoomSpeed += zoomSpeedBoost * inputs.mouseScroll.y;
        }
        if (inputs.mouseScroll.y != 0 && params->zoom > 1) {
            params->zoomCenter = (Vector2) {
                .x = inputs.mousePos.x - viewport.x,
                .y = inputs.mousePos.y - viewport.y
            };
        }
    }

    if (params->zoomSpeed >= 0) {
        params->zoom *= 1 + params->zoomSpeed;
    }
    else {
        params->zoom /= 1 - params->zoomSpeed;
    }
    params->zoom = max(min(params->zoom, 300), 0.1);
}

static void updateMainImageParams(ApplicationState* state, InputInfo inputs) {
    RenderParams_MainImage* params = &state->imageRenderParams;
    Texture2D tex = state->currentImage.texture;
    Clay_BoundingBox viewport =
        Clay_GetElementData(Clay_GetElementId(mkClayString("ImageContainer"))).boundingBox;

    float oldZoom = params->zoom;
    updateZoom(params, inputs, viewport);

    float vpAspectRatio = viewport.width / viewport.height;
    float imgAspectRatio = (float)tex.width / tex.height;
    float zoomConversion = 
        vpAspectRatio > imgAspectRatio ?
            viewport.height / tex.height :
            viewport.width / tex.width;
    
    // "1x zoom" means image fills whole viewport -> need to express zoom in terms of
    // enlarging/shrinking the image to fit the box
    float absZoom = params->zoom * zoomConversion;

    Rectangle virtImgSize = {
        .width = tex.width * absZoom,
        .height = tex.height * absZoom
    };

    static Rectangle virtImgCrop;
    virtImgCrop.x = (virtImgCrop.x + params->zoomCenter.x) * params->zoom / oldZoom - params->zoomCenter.x;
    virtImgCrop.y = (virtImgCrop.y + params->zoomCenter.y) * params->zoom / oldZoom - params->zoomCenter.y;
    virtImgCrop.width = min(virtImgSize.width, viewport.width);
    virtImgCrop.height = min(virtImgSize.height, viewport.height);

    if (state->pointerDraggingState == POINTER_DEFAULT &&
            inputs.mouseLeftDown &&
            CheckCollisionPointRec(inputs.mousePos, params->imageTarget) &&
            (virtImgSize.height > viewport.height || virtImgSize.width > viewport.width)) {
        state->pointerDraggingState = POINTER_DRAGGING_IMAGE;
    }

    // virtImgCrop.width = min(virtImgSize.width, viewport.width);
    // virtImgCrop.height = min(virtImgSize.height, viewport.height);

    if (state->pointerDraggingState == POINTER_DRAGGING_IMAGE) {
        virtImgCrop.x -= inputs.mouseDelta.x;
        virtImgCrop.y -= inputs.mouseDelta.y;
    }
    // if (params->isDragging) {
    //     params->zoomSpeed = 0;
    // }

    virtImgCrop.x = clamp(virtImgCrop.x, 0, virtImgSize.width - virtImgCrop.width);
    virtImgCrop.y = clamp(virtImgCrop.y, 0, virtImgSize.height - virtImgCrop.height);
    
    params->imageCrop = (Rectangle){
        .width =
            virtImgSize.width > viewport.width ?
                (viewport.width / virtImgSize.width) * tex.width :
                tex.width,
        .height =
            virtImgSize.height > viewport.height ?
                (viewport.height / virtImgSize.height) * tex.height :
                tex.height,
        .x = virtImgCrop.x / absZoom,
        .y = virtImgCrop.y / absZoom
    };

    params->imageTarget = (Rectangle){
        .width = virtImgCrop.width,
        .height = virtImgCrop.height,
        .x = viewport.x + (viewport.width - virtImgCrop.width) / 2,
        .y = viewport.y + (viewport.height - virtImgCrop.height) / 2,
    };
}

void updatePointer(PointerState pointerState) {
    switch (pointerState) {
        case POINTER_DEFAULT:
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            break;
        case POINTER_DRAGGING_IMAGE:
            SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
            break;
        case POINTER_HOVERING_SIDEBAR:
        case POINTER_DRAGGING_SIDEBAR:
            SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
            break;
        default:
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

void nextImage(ApplicationState* state) {
    if (state->currentImgIndex == state->imgFilesInDir.count - 1) {
        return;
    }

    freeImage(state->currentImage);
    state->currentImgIndex++;
    state->currentImage = loadImage(state->imgFilesInDir.paths[state->currentImgIndex]);
    state->imageRenderParams.imageCrop = (Rectangle){
        .x = 0, .y = 0, .width = state->currentImage.texture.width, .height = state->currentImage.texture.height};
    SetWindowTitle(state->currentImage.metadata.fileName);
}

void previousImage(ApplicationState* state) {
    if (state->currentImgIndex == 0) {
        return;
    }

    freeImage(state->currentImage);
    state->currentImgIndex--;
    state->currentImage = loadImage(state->imgFilesInDir.paths[state->currentImgIndex]);
    state->imageRenderParams.imageCrop = (Rectangle){
        .x = 0, .y = 0, .width = state->currentImage.texture.width, .height = state->currentImage.texture.height};
    SetWindowTitle(state->currentImage.metadata.fileName);
}

void updateState(ApplicationState *state, InputInfo inputs) {
    if ((isButtonPressed(inputs, "NavButtonLeft") || IsKeyPressed(KEY_LEFT)) && state->currentImgIndex > 0) {
        previousImage(state);
    }
    if ((isButtonPressed(inputs, "NavButtonRight") || IsKeyPressed(KEY_RIGHT)) &&
        state->currentImgIndex < state->imgFilesInDir.count - 1) {
        nextImage(state);
    }
    if (isButtonPressed(inputs, "InfoSidebarButton") || IsKeyPressed(KEY_I)) {
        state->infoScreenOpen ^= true;
    }
    static int numframessincereturnedfromfullscreen = 0; // this is the worst code ever written
    if (numframessincereturnedfromfullscreen == 3) {
        SetWindowFocused(); numframessincereturnedfromfullscreen = 0;}
    if (numframessincereturnedfromfullscreen > 0) {numframessincereturnedfromfullscreen++;}
    if (IsKeyPressed(KEY_F11)) {
        if (!IsWindowFullscreen()) {
            SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE); // awful hack - GetScreenHeight *includes* the
                                            // size of window decorations, whereas SetWindowSize does not
            state->prevWindowDims = (Vector2) {GetScreenWidth(), GetRenderHeight()};
            // ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE); // clearing this flag immediately breaks
                                                                // everything
            int monitor = GetCurrentMonitor();
            SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
            ToggleFullscreen();
        }
        else {
            SetWindowSize(state->prevWindowDims.x, state->prevWindowDims.y);
            ToggleFullscreen();
            ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
            numframessincereturnedfromfullscreen++;
            // SetWindowFocused(); // exiting borderless window mode makes the window lose focus
                                // and this fucking call does nothing for some reason
        }
    }
    #ifdef DEBUG
    if (IsKeyPressed(KEY_D)) {
        Clay_SetDebugModeEnabled(!Clay_IsDebugModeEnabled());
    }
    #endif /* ifdef DEBUG */

    if (state->infoScreenOpen && isButtonPressed(inputs, "InfoSidebarHandle")) {
        state->pointerDraggingState = POINTER_DRAGGING_SIDEBAR;
    }
    else if (state->infoScreenOpen && isButtonHovered(inputs, "InfoSidebarHandle")) {
        state->pointerDraggingState = POINTER_HOVERING_SIDEBAR;
    }
    else if (!inputs.mouseLeftDown) {
        state->pointerDraggingState = POINTER_DEFAULT;
    }
    updateMainImageParams(state, inputs);
    updatePointer(state->pointerDraggingState);

    if (state->pointerDraggingState == POINTER_DRAGGING_SIDEBAR) {
        state->infoScreenWidth = clamp(GetRenderWidth() - inputs.mousePos.x, 256, 800);
    }
    state->infoScreenWidth = min(state->infoScreenWidth, GetRenderWidth() - 300);

    if (inputs.mouseDelta.x == 0 && inputs.mouseDelta.y == 0) {
        state->timeSinceMouseMovement += inputs.deltaTime;
    }
    else {
        state->timeSinceMouseMovement = 0;
    }
}
