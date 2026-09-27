#include "state.h"

#include "image.h"
#include "io.h"

ApplicationState initAppState(char* imagePath) {
    ApplicationState s;
    const char* fileDir = GetDirectoryPath(imagePath);
    s.imgFilesInDir = getImagePaths(fileDir);
    s.currentImgIndex = findInImagePaths(s.imgFilesInDir, imagePath);
    s.currentImage = loadImage(imagePath);
    return s;
}

void nextImage(ApplicationState* state) {
    if (state->currentImgIndex == state->imgFilesInDir.count - 1) {
        return;
    }

    freeImage(state->currentImage);
    state->currentImgIndex++;
    state->currentImage = loadImage(state->imgFilesInDir.paths[state->currentImgIndex]);
    SetWindowTitle(state->currentImage.metadata.fileName);
}

void previousImage(ApplicationState* state) {
    if (state->currentImgIndex == 0) {
        return;
    }

    freeImage(state->currentImage);
    state->currentImgIndex--;
    state->currentImage = loadImage(state->imgFilesInDir.paths[state->currentImgIndex]);
    SetWindowTitle(state->currentImage.metadata.fileName);
}
