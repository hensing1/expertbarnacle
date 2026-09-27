#ifndef INCLUDE_SRC_STATE_H_
#define INCLUDE_SRC_STATE_H_

#include <stdbool.h>

#include <raylib.h>

#include "image.h"

typedef struct {
    bool infoScreenOpen;
    FilePathList imgFilesInDir;
    int currentImgIndex;
    ImageData currentImage;
} ApplicationState;

ApplicationState initAppState(char* imagePath);

// --- everything to do with altering the app state ---

void nextImage(ApplicationState* state);
void previousImage(ApplicationState* state);

#endif  // INCLUDE_SRC_STATE_H_
