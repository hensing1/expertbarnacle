#ifndef INCLUDE_SRC_IO_H_
#define INCLUDE_SRC_IO_H_

#include <stddef.h>
#include <time.h>

#include <raylib.h>

typedef struct {
    int height;
    int width;
    const char* fileName;
    char* fullPath;
    size_t sizeBytes;
    char* sizeStr;
    time_t timeModifiedUnix;  // unix time stamp, in seconds
    char* timeModifiedStr;
} ImageInfo;

typedef struct {
    float deltaTime;
    Vector2 mousePos;
    Vector2 mouseDelta;
    Vector2 mouseScroll;
    bool mouseLeftPressed;
    bool mouseRightPressed;
    bool mouseLeftDown;
    bool mouseRightDown;
} InputInfo;

InputInfo captureInputs();
bool isFile(char* path);
ImageInfo loadImageInfo(Image image, const char* filepath);
void freeImageInfo(ImageInfo image);
FilePathList getImagePaths(const char* directory);
int findInImagePaths(FilePathList paths, char* file);

#endif  // INCLUDE_SRC_IO_H_
