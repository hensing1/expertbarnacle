#include "ui.h"

#include <stdio.h>

#include <raylib.h>
#include <string.h>
#include "lib/clay.h"

#include "colors.h"
#include "format.h"
#include "state.h"

Clay_Vector2 convVec2RaylibClay(Vector2 vec) {return (Clay_Vector2){vec.x, vec.y};}

void infoBox(Clay_String id, int index, Clay_String title, Clay_String content) {
    CLAY(CLAY_SIDI_LOCAL(id, index), {
        .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT() },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = 10 },
        .backgroundColor = C_BLUE,
    }) {
        CLAY_TEXT(title, { .textColor = C_LIGHTGRAY, .fontSize = 32 });
        CLAY_TEXT(content, { .textColor = C_WHITE, .fontSize = 36 });
    }
}

Clay_RenderCommandArray createUI(ApplicationState state, InputInfo inputs, Clay_String* strings) {
    Clay_SetLayoutDimensions((Clay_Dimensions){ GetScreenWidth(), GetScreenHeight() });
    Clay_SetPointerState(convVec2RaylibClay(inputs.mousePos), inputs.mouseLeftPressed);
    Clay_UpdateScrollContainers(true, convVec2RaylibClay(inputs.mouseScroll), inputs.deltaTime);

    Clay_BeginLayout();
    CLAY(CLAY_ID("Global"), {
        .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} },
        .backgroundColor = C_SLATE
    }) {
        // CLAY(CLAY_ID("MenuBar"), {
        //     .layout = { .sizing = {CLAY_SIZING_GROW(0), 64 }},
        //     .backgroundColor = BLACK
        // }) {
        // }
        CLAY(CLAY_ID("ImageContainer"), {
            .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)} },
        }) {
            // CLAY(CLAY_ID("PrevImage"), {
            //     .layout = { .sizing = {CLAY_SIZING_FIXED(64), CLAY_SIZING_FIXED(64)} },
            //     .floating = { .attachTo = CLAY_ATTACH_TO_PARENT,
            //                   .attachPoints = { .element = CLAY_ATTACH_POINT_LEFT_CENTER, .parent = CLAY_ATTACH_POINT_LEFT_CENTER},
            //                   .offset.x = 32 },
            //     .backgroundColor = GREEN
            // }) {}
            // CLAY(CLAY_ID("NextImage"), {
            //     .layout = { .sizing = {CLAY_SIZING_FIXED(64), CLAY_SIZING_FIXED(64)} },
            //     .floating = { .attachTo = CLAY_ATTACH_TO_PARENT,
            //                   .attachPoints = { .element = CLAY_ATTACH_POINT_RIGHT_CENTER, .parent = CLAY_ATTACH_POINT_RIGHT_CENTER},
            //                   .offset.x = -32 },
            //     .backgroundColor = GREEN
            // }) {}
        }
       
        if (state.infoScreenOpen) {
            CLAY(CLAY_ID("InfoSidebar"), {
                .layout = { .sizing = {CLAY_SIZING_FIXED(400), CLAY_SIZING_GROW(0)},
                            .padding = CLAY_PADDING_ALL(5) },
            }) {
                CLAY(CLAY_ID("InfoSidebarInner"), {
                    .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                                .padding = CLAY_PADDING_ALL(16),
                                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                .childGap = 16},
                    .border = { .width = CLAY_BORDER_OUTSIDE(2),
                                .color = C_GRAY },
                    // .cornerRadius = CLAY_CORNER_RADIUS(10)
                }) {
                    infoBox(CLAY_STRING("InfoSidebarBox"), 1, CLAY_STRING("och nö"), CLAY_STRING("och nö"));
                    // infoBox(CLAY_STRING("InfoSidebarBox"), 2, strings[STR_FILESIZE_TITLE], strings[STR_FILESIZE]);
                    // infoBox(CLAY_STRING("InfoSidebarBox"), 3, strings[STR_TIME_MODIFIED_TITLE], strings[STR_TIME_MODIFIED]);
                }
            }
        }

    }
    return Clay_EndLayout(inputs.deltaTime);
}
