#ifndef INCLUDE_SRC_STATE_H_
#define INCLUDE_SRC_STATE_H_

#include <stdbool.h>

#include <raylib.h>

typedef struct {
    bool infoScreenOpen;
    FilePathList imgFilesInDir;
    int currentImgIndex;
} ApplicationState;

ApplicationState initAppState(char* imagePath);

#endif  // INCLUDE_SRC_STATE_H_
