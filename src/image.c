#include "image.h"

#include <raylib.h>

#include "format.h"
#include "io.h"

ImageData loadImage(const char* imagePath) {
    Texture2D imageTexture = LoadTexture(imagePath);
    GenTextureMipmaps(&imageTexture);
    SetTextureWrap(imageTexture, TEXTURE_WRAP_MIRROR_REPEAT);
    SetTextureFilter(imageTexture, TEXTURE_FILTER_TRILINEAR);

    Arena imageArena = {};
    ImageMetadata meta = loadImageMetadata(&imageArena, imageTexture, imagePath);
    Clay_String* imgStrings = makeImageStrings(&imageArena, LANG_DE, meta);

    return (ImageData) {
        .arena = imageArena,
        .texture = imageTexture,
        .metadata = meta,
        .strings = imgStrings
    };
}

void freeImage(ImageData imageData) {
    arena_free(&imageData.arena);
    UnloadTexture(imageData.texture);
}
