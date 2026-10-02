#include "ui.h"

#include <raylib.h>
#include "lib/clay.h"

#include "colors.h"
#include "format.h"
#include "state.h"

Clay_Vector2 convVec2RaylibClay(Vector2 vec) {return (Clay_Vector2){vec.x, vec.y};}

void infoBox(Clay_String id, int index, Clay_String title, Clay_String content) {
    CLAY(CLAY_SIDI_LOCAL(id, index), {
        .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT() },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = CLAY_PADDING_ALL(10),
                    .childGap = 10 },
        // .border = { .width = CLAY_BORDER_ALL(2),
        //             .color = C_GRAY },
        .backgroundColor = C_SLATE,
    }) {
        CLAY_TEXT(title, { .textColor = C_LIGHTGRAY, .fontId = 0, .fontSize = 28 });
        CLAY_TEXT(content, { .textColor = C_WHITE, .fontId = 1, .fontSize = 32 });
    }
}

Clay_RenderCommandArray createUI(const ApplicationState state, const InputInfo inputs, Arena* frameArena) {
    Clay_String* strings = state.currentImage.strings;
    Clay_SetLayoutDimensions((Clay_Dimensions){ GetScreenWidth(), GetScreenHeight() });
    Clay_SetPointerState(convVec2RaylibClay(inputs.mousePos), inputs.mouseLeftPressed);
    Clay_UpdateScrollContainers(true, convVec2RaylibClay(inputs.mouseScroll), inputs.deltaTime);

    Clay_BeginLayout();
    CLAY(CLAY_ID("Global"), {
        .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()} },
        .backgroundColor = C_SLATE
    }) {
        CLAY(CLAY_ID("ImageContainer"), {
            .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()},
                        .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
            },
        }) {
            CustomLayoutElement* custom = arena_alloc(frameArena, sizeof(CustomLayoutElement));
            *custom = (CustomLayoutElement){
                .type = CUSTOM_LAYOUT_ELEMENT_TYPE_MAINIMAGE,
                .customData.imageRenderData = {
                    .image = state.currentImage.texture,
                    .imageRenderParams = state.imageRenderParams
                }
            };
            CLAY(CLAY_ID("Image"), {
                .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()} },
                .custom = { .customData = custom }
            });
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
                .layout = { .sizing = {CLAY_SIZING_FIXED(state.infoScreenWidth), CLAY_SIZING_GROW()},
                            .padding = {3, 0, 0, 0} },
            }) {
                CLAY(CLAY_ID("InfoSidebarHandle"), {
                    .layout = { .sizing = {CLAY_SIZING_FIXED(16), CLAY_SIZING_GROW()}},
                    .backgroundColor = {30, 29, 36, 255}
                });
                CLAY(CLAY_ID("InfoSidebarInner"), {
                    .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()},
                                .padding = {.top = 16, .right = 16, .bottom = 16, .left = 0},
                                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                .childGap = 16},
                    .backgroundColor = {30, 29, 36, 255}
                }) {
                    infoBox(CLAY_STRING("InfoSidebarBox"), 1, strings[STR_RESOLUTION_TITLE], strings[STR_RESOLUTION]);
                    infoBox(CLAY_STRING("InfoSidebarBox"), 2, strings[STR_FILESIZE_TITLE], strings[STR_FILESIZE]);
                    infoBox(CLAY_STRING("InfoSidebarBox"), 3, strings[STR_TIME_MODIFIED_TITLE], strings[STR_TIME_MODIFIED]);
                    CLAY(CLAY_ID("InfoSidebarBalloon"), { 
                        .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()} }
                    });
                    CLAY(CLAY_ID("InfoSidebarNavContainer"), { 
                        .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_FIT()} }
                    }) {
                        CLAY(CLAY_ID("NavButtonLeft"), {
                            .layout = { .sizing = {CLAY_SIZING_FIXED(80), CLAY_SIZING_FIXED(80)},
                                        .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER} },
                            .border = { .width = CLAY_BORDER_ALL(2),
                                        .color = C_GRAY },
                            .backgroundColor = {80, 80, 80, Clay_Hovered() ? 255 : 0}
                        }) {
                            CLAY_TEXT(CLAY_STRING("<"), { .textColor = C_LIGHTGRAY, .fontId = 0, .fontSize = 28 });
                        }
                        CLAY(CLAY_ID("NavContainerBalloon"), { 
                            .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()} }
                        });
                        CLAY(CLAY_ID("NavButtonRight"), {
                            .layout = { .sizing = {CLAY_SIZING_FIXED(80), CLAY_SIZING_FIXED(80)},
                                        .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER} },
                            .border = { .width = CLAY_BORDER_ALL(2),
                                        .color = C_GRAY },
                            .backgroundColor = {80, 80, 80, Clay_Hovered() ? 255 : 0}
                        }) {
                            CLAY_TEXT(CLAY_STRING(">"), { .textColor = C_LIGHTGRAY, .fontId = 0, .fontSize = 28 });
                        }
                    }
                }
            }
        }
    }
    return Clay_EndLayout(inputs.deltaTime);
}
