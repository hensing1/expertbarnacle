#include "io.h"

#include <glob.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <raylib.h>

#include "format.h"
#include "util.h"

#define IMAGE_TYPES ".png;.bmp;.tga;.jpg;.jpeg;.gif;.qoi;.psd;.dds;.hdr;.ktx;.astc;.pkm;.pvr"


InputInfo captureInputs() {
    return (InputInfo){
        .deltaTime = GetFrameTime(),
        .mousePos = GetMousePosition(),
        .mouseDelta = GetMouseDelta(),
        .mouseScroll = GetMouseWheelMoveV(),
        .mouseLeftPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
        .mouseRightPressed = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT),
        .mouseLeftDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT),
        .mouseRightDown = IsMouseButtonDown(MOUSE_BUTTON_RIGHT)
    };
}

bool isFile(char* file) {
    struct stat s;
    return stat(file, &s) == 0 && (s.st_mode & S_IFREG);
}

ImageInfo loadImageInfo(Image image, const char* filepath) {
    struct stat filestat;
    stat(filepath, &filestat);

    char* fullPath = malloc(PATH_MAX + 1);
    realpath(filepath, fullPath);

    return (ImageInfo) {
        .width = image.width,
        .height = image.height,
        .fileName = GetFileName(filepath),
        .fullPath = fullPath,
        .sizeBytes = filestat.st_size,
        .timeModifiedUnix = filestat.st_mtim.tv_sec,
    };
}

void freeImageInfo(ImageInfo image) {
    FREE_PTR(image.fullPath);
}

FilePathList getImagePaths(const char* directory) {
    FilePathList files = LoadDirectoryFilesEx(directory, IMAGE_TYPES, false);
    sortAlNumCaseInsensitive(files.paths, files.count);
    return files;
}

int findInImagePaths(FilePathList paths, char* file) {
    struct stat needle;
    stat(file, &needle);
    for (int i = 0; i < paths.count; i++) {
        struct stat hay;
        stat(paths.paths[i], &hay);
        if (hay.st_ino == needle.st_ino) {
            return i;
        }
    }
    return -1;
}
