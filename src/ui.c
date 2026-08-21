#include "ui.h"

#include <raylib.h>
#include "lib/clay.h"
#include "colors.h"

Clay_Vector2 convVec2RaylibClay(Vector2 vec) {return (Clay_Vector2){vec.x, vec.y};}

Clay_RenderCommandArray createUI(InputInfo inputs) {
    Clay_SetLayoutDimensions((Clay_Dimensions){ GetScreenWidth(), GetScreenHeight() });
    Clay_SetPointerState(convVec2RaylibClay(inputs.mousePos), inputs.mouseLeftPressed);
    Clay_UpdateScrollContainers(true, convVec2RaylibClay(inputs.mouseScroll), inputs.deltaTime);

    Clay_BeginLayout();
    CLAY(CLAY_ID("Global"), {
        .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} },
        .backgroundColor = SLATE
    }) {
        // CLAY(CLAY_ID("MenuBar"), {
        //     .layout = { .sizing = {CLAY_SIZING_GROW(0), 64 }},
        //     .backgroundColor = BLACK
        // }) {
        // }
        CLAY(CLAY_ID("ImageContainer"), {
            .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}},
        }) {
            CLAY(CLAY_ID("PrevImage"), {
                .layout = { .sizing = {CLAY_SIZING_FIXED(64), CLAY_SIZING_FIXED(64)} },
                .floating = { .attachTo = CLAY_ATTACH_TO_PARENT,
                              .attachPoints = { .element = CLAY_ATTACH_POINT_LEFT_CENTER, .parent = CLAY_ATTACH_POINT_LEFT_CENTER},
                              .offset.x = 32 },
                .backgroundColor = GREEN
            }) {}
            CLAY(CLAY_ID("NextImage"), {
                .layout = { .sizing = {CLAY_SIZING_FIXED(64), CLAY_SIZING_FIXED(64)} },
                .floating = { .attachTo = CLAY_ATTACH_TO_PARENT,
                              .attachPoints = { .element = CLAY_ATTACH_POINT_RIGHT_CENTER, .parent = CLAY_ATTACH_POINT_RIGHT_CENTER},
                              .offset.x = -32 },
                .backgroundColor = GREEN
            }) {}
        }
    }
    return Clay_EndLayout(inputs.deltaTime);
}
