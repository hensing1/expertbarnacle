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
        CustomLayoutElement* custom = arena_alloc(frameArena, sizeof(CustomLayoutElement));
        *custom = (CustomLayoutElement){
            .type = CUSTOM_LAYOUT_ELEMENT_TYPE_MAINIMAGE,
            .customData.imageRenderData = {
                .image = state.currentImage.texture,
                .imageRenderParams = state.imageRenderParams
            }
        };
        CLAY(CLAY_ID("ImageContainer"), {
            .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()},
                        .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },
            },
            .custom = { .customData = custom },
        }) {
            float overlayAlpha = state.isImageUiVisible ? 255 : 0;
            Clay_TransitionElementConfig overlayTransition = {
                .handler = Clay_EaseOut,
                .properties = CLAY_TRANSITION_PROPERTY_BACKGROUND_COLOR,
                .duration = state.isImageUiVisible ? 0.1f : 0.8f
            };
            
            CLAY(CLAY_ID("NavButtonLeft"), {
                .layout = { .sizing = {CLAY_SIZING_FIXED(64), CLAY_SIZING_FIXED(64)},
                            .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}},
                .floating = { .attachTo = CLAY_ATTACH_TO_PARENT,
                              .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                              .attachPoints = { .element = CLAY_ATTACH_POINT_LEFT_CENTER,
                                                .parent = CLAY_ATTACH_POINT_LEFT_CENTER},
                                                .offset.x = 32 },
                .border = { .width = CLAY_BORDER_ALL(2),
                            .color = CHANGE_ALPHA(C_GRAY, overlayAlpha) },
                .backgroundColor = Clay_Hovered() ? 
                    (Clay_Color){80, 80, 80, overlayAlpha} :
                    CHANGE_ALPHA(C_SLATE, overlayAlpha),
                .transition = overlayTransition
            }) {
                CLAY_TEXT(CLAY_STRING("<"),
                    { .textColor = CHANGE_ALPHA(C_LIGHTGRAY, overlayAlpha), .fontId = 0, .fontSize = 28 });
            }
            CLAY(CLAY_ID("NavButtonRight"), {
                .layout = { .sizing = {CLAY_SIZING_FIXED(64), CLAY_SIZING_FIXED(64)},
                            .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}},
                .floating = { .attachTo = CLAY_ATTACH_TO_PARENT,
                              .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                              .attachPoints = { .element = CLAY_ATTACH_POINT_RIGHT_CENTER,
                                                .parent = CLAY_ATTACH_POINT_RIGHT_CENTER},
                                                .offset.x = -32 },
                .border = { .width = CLAY_BORDER_ALL(2),
                            .color = CHANGE_ALPHA(C_GRAY, overlayAlpha) },
                .backgroundColor = Clay_Hovered() ? 
                    (Clay_Color){80, 80, 80, overlayAlpha} :
                    CHANGE_ALPHA(C_SLATE, overlayAlpha),
                .transition = overlayTransition
            }) {
                CLAY_TEXT(CLAY_STRING(">"),
                    { .textColor = CHANGE_ALPHA(C_LIGHTGRAY, overlayAlpha), .fontId = 0, .fontSize = 28 });
            }
            CLAY(CLAY_ID("InfoSidebarButton"), {
                .layout = { .sizing = {CLAY_SIZING_FIT(), CLAY_SIZING_FIT()},
                            .padding = CLAY_PADDING_ALL(8)},
                .floating = { .attachTo = CLAY_ATTACH_TO_PARENT,
                              .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                              .attachPoints = { .element = CLAY_ATTACH_POINT_RIGHT_TOP,
                                                .parent = CLAY_ATTACH_POINT_RIGHT_TOP},
                                                .offset = {-16, 16} },
                .border = { .width = CLAY_BORDER_ALL(2),
                            .color = CHANGE_ALPHA(C_GRAY, overlayAlpha) },
                .backgroundColor = Clay_Hovered() ? 
                    (Clay_Color){80, 80, 80, overlayAlpha} :
                    CHANGE_ALPHA(C_SLATE, overlayAlpha),
                .transition = overlayTransition
            }) {
                CLAY_TEXT(state.isSidebarOpen ? strings[STR_INFO_OPEN]: strings[STR_INFO_CLOSED],
                    { .textColor = CHANGE_ALPHA(C_LIGHTGRAY, overlayAlpha), .fontId = 0, .fontSize = 28 });
            }
        }
       
        if (state.isSidebarOpen) {
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
                }
            }
        }
    }
    return Clay_EndLayout(inputs.deltaTime);
}
