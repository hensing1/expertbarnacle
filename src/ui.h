#ifndef INCLUDE_SRC_UI_H_
#define INCLUDE_SRC_UI_H_

#include "lib/arena.h"
#include "lib/clay.h"
#include "state.h"

typedef enum {
    CUSTOM_LAYOUT_ELEMENT_TYPE_MAINIMAGE
} CustomLayoutElementType;

typedef struct {
    Texture2D image;
    RenderParams_MainImage imageRenderParams;
} RenderData_MainImage;

typedef struct {
    CustomLayoutElementType type;
    union {
        RenderData_MainImage imageRenderData;
    } customData;
} CustomLayoutElement;


Clay_RenderCommandArray createUI(const ApplicationState state, const InputInfo inputs, Arena* frameArena);

#endif  // INCLUDE_SRC_UI_H_
