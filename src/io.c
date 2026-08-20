#include "io.h"

#include <glob.h>
#include <raylib.h>
#include <stdio.h>

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

FilePathList getImageFiles(const char* directory) {
    FilePathList files = LoadDirectoryFilesEx(directory, IMAGE_TYPES, false);
    sortAlNumCaseInsensitive(files.paths, files.count);
    return files;
}
