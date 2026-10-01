#ifndef INCLUDE_SRC_STATE_H_
#define INCLUDE_SRC_STATE_H_

#include <stdbool.h>

#include <raylib.h>

#include "image.h"

typedef struct {
    float zoom;
    float zoomSpeed;
    Vector2 zoomCenter;
    bool isDragging;
    Rectangle imageCrop;
    Rectangle imageTarget;
} RenderParams_MainImage;

typedef struct {
    bool infoScreenOpen;
    FilePathList imgFilesInDir;
    int currentImgIndex;
    ImageData currentImage;

    RenderParams_MainImage imageRenderParams;
} ApplicationState;

ApplicationState initAppState(char* imagePath);

// --- everything to do with altering the app state ---
void updateState(ApplicationState* state, InputInfo inputs);
void nextImage(ApplicationState* state);
void previousImage(ApplicationState* state);

void setImageRect(Rectangle r, Rectangle s);

#endif  // INCLUDE_SRC_STATE_H_
