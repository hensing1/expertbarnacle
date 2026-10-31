#ifndef INCLUDE_SRC_IO_H_
#define INCLUDE_SRC_IO_H_

#include <stddef.h>
#include <time.h>

#include <raylib.h>

#include "lib/arena.h"

typedef struct {
    int height;
    int width;
    const char* fileName;
    char* fullPath;
    size_t sizeBytes;
    time_t timeModifiedUnix;  // unix time stamp, in seconds
} ImageMetadata;

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

typedef enum {
    JSON_TYPE_NULL,
    JSON_TYPE_BOOL,
    JSON_TYPE_NUMBER,
    JSON_TYPE_STRING,
    JSON_TYPE_ARRAY,
    JSON_TYPE_OBJECT,
    JSON_TYPE_PROPERTY
} JsonType;

typedef struct _JsonElement JsonElement;

typedef struct {
    JsonElement* elements;
    size_t numElements;
    size_t capacity;
} JsonArray;

typedef struct {
    char* key;
    JsonElement* value;
} JsonProperty;

typedef struct {
    JsonProperty* props;
    size_t numProps;
    size_t capacity;
} JsonObject;

typedef struct _JsonElement {
    JsonType type;
    union {
        bool boolValue;
        float numberValue;
        char* stringValue;
        JsonArray arrayValue;
        JsonObject objectValue;
    } value;
} JsonElement;

InputInfo captureInputs();
bool isFile(char* path);
ImageMetadata loadImageMetadata(Arena* imgArena, Texture2D image, const char* filepath);
size_t loadFileAsBytes(const char* filepath, unsigned char** data);
FilePathList getImagePaths(const char* directory);
int findInImagePaths(FilePathList paths, char* file);

#endif  // INCLUDE_SRC_IO_H_
