#ifndef INCLUDE_SRC_IMAGE_H_
#define INCLUDE_SRC_IMAGE_H_

#include <raylib.h>

#include "lib/arena.h"
#include "lib/clay.h"
#include "io.h"

typedef struct {
    Arena arena;
    Texture2D texture;
    ImageMetadata metadata;
    Clay_String* strings;
} ImageData;

ImageData loadImage(const char* imagePath);
void freeImage(ImageData imageData);

#endif  // INCLUDE_SRC_IMAGE_H_
