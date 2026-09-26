#include "image.h"
#include <raylib.h>

ImageData loadImage(const char* imagePath) {
    
}

void freeImage(ImageData imageData) {
    arena_free(&imageData.arena);
    UnloadImage(imageData.image);
}
