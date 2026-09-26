#ifndef INCLUDE_SRC_UI_H_
#define INCLUDE_SRC_UI_H_

#include "lib/clay.h"
#include "render.h"
#include "state.h"

typedef enum {
    CUSTOM_LAYOUT_ELEMENT_TYPE_MAINIMAGE
} CustomLayoutElementType;

typedef struct {
    float zoom;
} CustomLayoutElement_MainImage;

typedef struct {
    CustomLayoutElementType type;
    union {
        CustomLayoutElement_MainImage imageRenderParams;
    } customData;
} CustomLayoutElement;


Clay_RenderCommandArray createUI(ApplicationState state, InputInfo inputs, Clay_String* strings);

#endif  // INCLUDE_SRC_UI_H_
