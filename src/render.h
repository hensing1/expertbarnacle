#ifndef INCLUDE_SRC_RENDER_H_
#define INCLUDE_SRC_RENDER_H_

#include <raylib.h>

#include "io.h"

// float getImageZoomLevel();
void renderImage(Texture2D image, Shader shader, Rectangle boundingBox, InputInfo inputs);

#endif  // INCLUDE_SRC_RENDER_H_
