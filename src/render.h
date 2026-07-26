#ifndef INCLUDE_SRC_RENDER_H_
#define INCLUDE_SRC_RENDER_H_

#include "raylib.h"

typedef struct {
    Vector2 mousePos;
    Vector2 mouseScroll;
    bool mouseLeft;
    bool mouseRight;
} InputInfo;

float getImageZoomLevel();
void renderImage(Texture2D image, Shader shader, Rectangle boundingBox, InputInfo inputs);

#endif  // INCLUDE_SRC_RENDER_H_
