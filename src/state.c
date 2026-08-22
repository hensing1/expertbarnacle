#include "state.h"

#include "io.h"

ApplicationState initAppState(char* imagePath) {
    ApplicationState s;
    const char* fileDir = GetDirectoryPath(imagePath);
    s.imgFilesInDir = getImagePaths(fileDir);
    s.currentImgIndex = findInImagePaths(s.imgFilesInDir, imagePath);
    return s;
}
