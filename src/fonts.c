#include "fonts.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"

typedef struct {
    unsigned char* fontData;
    char* fontDataFileType;
    int fontDataByteCount;
    Font* sizes;
    bool* fontLoaded;
    int minSize;
    int maxSize;
} MultisizeFont;

static MultisizeFont font = {};

void initFont(char* fontFilePath) {
    char* fileType = NULL;
    for (int i = 0; fontFilePath[i]; i++) {
        if (fontFilePath[i] == '.') {
            fileType = fontFilePath + i;
        }
    }
    if (!fileType) {
        fprintf(stderr, "Could not find font file extension\n");
        exit(-1);
    }
    font.fontDataFileType = malloc(strlen(fileType)+1);
    strcpy(font.fontDataFileType, fileType);

    font.minSize = 5;
    font.maxSize = 50;
    font.sizes = malloc(sizeof(Font) * (font.maxSize - font.minSize));
    font.fontLoaded = malloc(sizeof(bool) * (font.maxSize - font.minSize));

    font.fontDataByteCount = loadFileAsBytes(fontFilePath, &font.fontData);
}

Font getFont(int size) {
    if (size > font.maxSize) {
        fprintf(stderr, "größe %i ham wa nich\n", size);
        exit(-1);
    }
    int i = size - font.minSize;
    if (!font.fontLoaded[i]) {
        font.sizes[i] = LoadFontFromMemory(font.fontDataFileType, font.fontData, font.fontDataByteCount, size, NULL, 250);
        if (!IsFontValid(font.sizes[i])) {
            fprintf(stderr, "Could not load font size %i\n", size);
            exit(-1);
        }
        font.fontLoaded[i] = true;
    }
    return font.sizes[i];
}
