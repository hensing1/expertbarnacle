#ifndef INCLUDE_SRC_RENDERER_H_
#define INCLUDE_SRC_RENDERER_H_

#include <raylib.h>

#include "lib/clay.h"
#include "state.h"

typedef enum {
    CUSTOM_LAYOUT_ELEMENT_TYPE_MAINIMAGE,
    CUSTOM_LAYOUT_ELEMENT_TYPE_ICON
} CustomLayoutElementType;

typedef struct {
    Texture2D image;
    RenderParams_MainImage imageRenderParams;
} RenderData_MainImage;

typedef struct {
    Texture2D* iconTexture;
    Vector4 foregroundColor;
    Vector4 backgroundColor;
} RenderData_Icon;

typedef struct {
    CustomLayoutElementType type;
    union {
        RenderData_MainImage imageRenderData;
        RenderData_Icon iconRenderData;
    } customData;
} CustomLayoutElement;

void InitOverlay();
void SetColorOverlay(Color color);
void DisableColorOverlay();

void initShaders();

void Clay_Raylib_Initialize(Font* fonts);
void Clay_Raylib_Close();
void Clay_Raylib_Render(Clay_RenderCommandArray renderCommands);

#endif  // INCLUDE_SRC_RENDERER_H_
