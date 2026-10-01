#ifndef INCLUDE_SRC_RENDERER_H_
#define INCLUDE_SRC_RENDERER_H_

#include <raylib.h>

#include "lib/clay.h"

void InitOverlay();
void SetColorOverlay(Color color);
void DisableColorOverlay();

void initImageShader();

void Clay_Raylib_Initialize(Font* fonts);
void Clay_Raylib_Close();
void Clay_Raylib_Render(Clay_RenderCommandArray renderCommands, Font* fonts);

#endif  // INCLUDE_SRC_RENDERER_H_
