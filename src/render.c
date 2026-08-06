#include <math.h>

#include <raylib.h>
#include <raymath.h>

#include "render.h"
#include "util.h"

// typedef enum {
//     MODE_STATIC,
//     MODE_ZOOMING,
//     MODE_DRAGGING
// } Mode;
//
// static Mode currentMode;

static const float zoomSpeedBoost = 0.02f;
static const float zoomSpeedDecay = 0.85f;

static float zoom = 1;
static float zoomSpeed = 0;

static Vector2 lastInputPos = {0, 0};

static Vector2 offset = {0, 0};
static bool isDragging = false;

static void handleZoom(InputInfo inputs) {
    if (fabsf(zoom - 1) <= 0.01 && zoomSpeed != 0) {  // snap to 1x zoom
        zoom = 1;
        zoomSpeed = 0;
        return;
    }
    if (sgn(zoomSpeed) == -sgn(inputs.mouseScroll.y)) {
        zoomSpeed = 0;
    }
    else {
        zoomSpeed *= powf(zoomSpeedDecay, 60.f * GetFrameTime());
        zoomSpeed += zoomSpeedBoost * inputs.mouseScroll.y;
    }
    if (zoomSpeed >= 0) {
        zoom *= 1 + zoomSpeed;
    }
    else {
        zoom /= 1 - zoomSpeed;
    }
    zoom = max(min(zoom, 300), 0.1);
}

void renderImage(Texture2D image, Shader shader, Rectangle boundingBox, InputInfo inputs) {
    if (!isDragging) {
        handleZoom(inputs);
        if (zoom > 1) {

        }
    }
    else {
        offset = Vector2Subtract(offset, GetMouseDelta());
    }

    float bbAspectRatio = boundingBox.width / boundingBox.height;
    float imgAspectRatio = (float)image.width / image.height;
    float zoomConversion = 
        bbAspectRatio > imgAspectRatio ?
            boundingBox.height / image.height :
            boundingBox.width / image.width;
    
    // "1x zoom" means image fills whole bounding box -> need to express zoom in terms of
    // enlarging/shrinking the image to fit the box
    float absZoom = zoom * zoomConversion;

    Rectangle virtualImageSize = {
        .width = image.width * absZoom,
        .height = image.height * absZoom
    };

    Rectangle imageTarget = {
        .width = min(virtualImageSize.width, boundingBox.width),
        .height = min(virtualImageSize.height, boundingBox.height),
    };  // virtualImage cropped
    // need to change this for proper zoom
    imageTarget.x = (boundingBox.width - imageTarget.width) / 2;
    imageTarget.y = (boundingBox.height - imageTarget.height) / 2;
    
    Rectangle imageCrop;
    imageCrop.width =
        virtualImageSize.width > boundingBox.width ?
            (boundingBox.width / virtualImageSize.width) * image.width :
            image.width;
    imageCrop.height =
        virtualImageSize.height > boundingBox.height ?
            (boundingBox.height / virtualImageSize.height) * image.height :
            image.height;

    Vector2 absOffset = Vector2Scale(offset, 1 / absZoom);
    absOffset.x = clamp(absOffset.x, 0, image.width - imageCrop.width);
    absOffset.y = clamp(absOffset.y, 0, image.height - imageCrop.height);

    imageCrop.x = absOffset.x;
    imageCrop.y = absOffset.y;

    offset = Vector2Scale(absOffset, absZoom);

    // render
    BeginShaderMode(shader);
    {
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

    isDragging = CheckCollisionPointRec(inputs.mousePos, imageTarget) &&
        (virtualImageSize.height > boundingBox.height || virtualImageSize.width > boundingBox.width) &&
        inputs.mouseLeft;

    if (isDragging) {
        zoomSpeed = 0;
        SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
    }
    else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

float getImageZoomLevel() { return zoom; }
