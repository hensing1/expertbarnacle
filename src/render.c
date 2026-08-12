#include <math.h>
#include <stdio.h>

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

typedef struct {
    Vector2 center;
    float relZoom; // relZoom == 1 <=> image fills viewport exactly
    // float abs; // absZoom == 1 <=> image resolution matches screen resolution
} ZoomInfo;

static const float zoomSpeedBoost = 0.02f;
static const float zoomSpeedDecay = 0.85f;

static float zoomSpeed = 0;

static void updateZoom(ZoomInfo* zoom, InputInfo inputs, Rectangle boundingBox) {
    static bool isDragging = false;

    if (fabsf(zoom->relZoom - 1) <= 0.01 && zoomSpeed != 0) {  // snap to 1x zoom
        zoom->relZoom = 1;
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
        zoom->relZoom *= 1 + zoomSpeed;
    }
    else {
        zoom->relZoom /= 1 - zoomSpeed;
    }
    zoom->relZoom = max(min(zoom->relZoom, 300), 0.1);

    if (inputs.mouseScroll.y != 0 && zoom->relZoom > 1) {
        zoom->center = (Vector2) {
            .x = inputs.mousePos.x - boundingBox.x,
            .y = inputs.mousePos.y - boundingBox.y
        };
    }
}

void renderImage(Texture2D image, Shader shader, Rectangle viewport, InputInfo inputs) {
    static ZoomInfo zoom = {.relZoom = 1};
    static Rectangle virtImgCrop = {.x = 0, .y = 0};
    static bool isDragging = false;
    
    float oldZoom = zoom.relZoom;
    if (!isDragging) {
        updateZoom(&zoom, inputs, viewport);
    }

    float vpAspectRatio = viewport.width / viewport.height;
    float imgAspectRatio = (float)image.width / image.height;
    float zoomConversion = 
        vpAspectRatio > imgAspectRatio ?
            viewport.height / image.height :
            viewport.width / image.width;
    
    // "1x zoom" means image fills whole viewport -> need to express zoom in terms of
    // enlarging/shrinking the image to fit the box
    float absZoom = zoom.relZoom * zoomConversion;

    Rectangle virtualImageSize = {
        .width = image.width * absZoom,
        .height = image.height * absZoom
    };

    virtImgCrop.width = min(virtualImageSize.width, viewport.width);
    virtImgCrop.height = min(virtualImageSize.height, viewport.height);

    if (isDragging) {
        virtImgCrop.x -= inputs.mouseDelta.x;
        virtImgCrop.y -= inputs.mouseDelta.y;
    }
    else {
        virtImgCrop.x = (virtImgCrop.x + zoom.center.x) * zoom.relZoom / oldZoom - zoom.center.x;
        virtImgCrop.y = (virtImgCrop.y + zoom.center.y) * zoom.relZoom / oldZoom - zoom.center.y;
    }
    virtImgCrop.x = clamp(virtImgCrop.x, 0, virtualImageSize.width - virtImgCrop.width);
    virtImgCrop.y = clamp(virtImgCrop.y, 0, virtualImageSize.height - virtImgCrop.height);
    
    Rectangle imageCrop;
    imageCrop.width =
        virtualImageSize.width > viewport.width ?
            (viewport.width / virtualImageSize.width) * image.width :
            image.width;
    imageCrop.height =
        virtualImageSize.height > viewport.height ?
            (viewport.height / virtualImageSize.height) * image.height :
            image.height;

    
    // imageCrop.x = clamp(virtImgCrop.x / absZoom, 0, image.width - imageCrop.width);
    // imageCrop.y = clamp(virtImgCrop.y / absZoom, 0, image.height - imageCrop.height);
    imageCrop.x = virtImgCrop.x / absZoom;
    imageCrop.y = virtImgCrop.y / absZoom;

    Rectangle imageTarget = {
        .width = virtImgCrop.width,
        .height = virtImgCrop.height,
        .x = (viewport.width - virtImgCrop.width) / 2,
        .y = (viewport.height - virtImgCrop.height) / 2,
    };

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
        (virtualImageSize.height > viewport.height || virtualImageSize.width > viewport.width) &&
        inputs.mouseLeft;

    if (isDragging) {
        zoomSpeed = 0;
        SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
    }
    else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

// float getImageZoomLevel() { return zoom; }
