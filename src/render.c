#include <math.h>

#include <raylib.h>

#include "render.h"
#include "util.h"

static float zoom = 1;
static float zoomSpeed = 0;
static const float zoomSpeedBoost = 0.04;
static const float zoomSpeedDecay = 0.9;

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
        zoomSpeed *= powf(zoomSpeedDecay, 60. * GetFrameTime());
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
    handleZoom(inputs);

    float bbAspectRatio = boundingBox.width / boundingBox.height;
    float imgAspectRatio = (float)image.width / image.height;
    float zoomConversion = 
        bbAspectRatio > imgAspectRatio ?
            boundingBox.height / image.height :
            boundingBox.width / image.width;
    
    float absZoom = zoom * zoomConversion; // zoom in terms of pixels

    Rectangle virtualImageSize = {
        .width = image.width * absZoom,
        .height = image.height * absZoom
    };
    virtualImageSize.x = (boundingBox.width - virtualImageSize.width) / 2;
    virtualImageSize.y = (boundingBox.height - virtualImageSize.height) / 2;

    Rectangle imageTarget = {
        .width = min(virtualImageSize.width, boundingBox.width),
        .height = min(virtualImageSize.height, boundingBox.height),
    }; //virtualImage cropped
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
    imageCrop.x = (image.width - imageCrop.width) / 2;
    imageCrop.y = (image.height - imageCrop.height) / 2;

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
}

float getImageZoomLevel() { return zoom; }
