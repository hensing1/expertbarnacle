#include "ui.h"

#include <raylib.h>
#include "lib/clay.h"

#include "colors.h"
#include "format.h"
#include "renderer.h"
#include "state.h"
#include "textures.h"
#include "util.h"

const Clay_TransitionElementConfig noTransition = {0};

Clay_Vector2 convVec2RaylibClay(Vector2 vec) {return (Clay_Vector2){vec.x, vec.y};}

void icon(Arena* frameArena, const char* id, IconID icon,
          Clay_Color foreground_default, Clay_Color foreground_hovered, Clay_Color background,
          Clay_TransitionElementConfig transition, int height) {
    Texture2D* iconTex = getIcon(icon);
    float aspectRatio = (float)iconTex->width / iconTex->height;
    CLAY(CLAY_SID(mkClayString(id)), {
        .layout = { .sizing = height > 0 ? (Clay_Sizing){ CLAY_SIZING_FIXED(aspectRatio * height), CLAY_SIZING_FIXED(height) } :
                                        (Clay_Sizing){ CLAY_SIZING_GROW(), CLAY_SIZING_GROW() } },
        .aspectRatio = { aspectRatio },
        .transition = transition,
    }) {
        Vector4 fg = Clay_Hovered() ? 
            (Vector4){.x = foreground_hovered.r / 255, .y = foreground_hovered.g / 255,
                      .z = foreground_hovered.b / 255, .w = foreground_hovered.a / 255} :
            (Vector4){.x = foreground_default.r / 255, .y = foreground_default.g / 255,
                      .z = foreground_default.b / 255, .w = foreground_default.a / 255};
        Vector4 bg = (Vector4){.x = background.r / 255, .y = background.g / 255, .z = background.b / 255, .w = background.a / 255};
        CustomLayoutElement* iconElement = arena_alloc(frameArena, sizeof(CustomLayoutElement));
        *iconElement = (CustomLayoutElement){
            .type = CUSTOM_LAYOUT_ELEMENT_TYPE_ICON,
            .customData.iconRenderData = (RenderData_Icon){
                .iconTexture = iconTex,
                .foregroundColor = fg,
                .backgroundColor = bg
            }
        };
        CLAY_AUTO_ID({
            .layout = { CLAY_SIZING_GROW(), CLAY_SIZING_GROW()},
            .custom = { .customData = (void*)iconElement },
            .transition = transition
        });
    }
}

void infoBox(Clay_String id, int index, float scale, Clay_String title, Clay_String content) {
    CLAY(CLAY_SIDI_LOCAL(id, index), {
        .layout = { .sizing = { CLAY_SIZING_GROW(0), CLAY_SIZING_FIT() },
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = CLAY_PADDING_ALL(10),
                    .childGap = 10 },
        // .border = { .width = CLAY_BORDER_ALL(2),
        //             .color = C_GRAY },
        .backgroundColor = C_SLATE,
    }) {
        CLAY_TEXT(title, { .textColor = C_LIGHTGRAY, .fontId = 0, .fontSize = 28 * scale });
        CLAY_TEXT(content, { .textColor = C_WHITE, .fontId = 1, .fontSize = 32 * scale });
    }
}

void overlay() {

}

void sidebar(const ApplicationState state, const InputInfo inputs, Arena* frameArena) {
    float s = state.scale;
    Clay_String* strings = state.currentImage.strings;
    CLAY(CLAY_ID("InfoSidebarHandle"), {
        .layout = { .sizing = {CLAY_SIZING_FIXED(16 * s), CLAY_SIZING_GROW()}},
        .backgroundColor = {30, 29, 36, 255}
    });
    CLAY(CLAY_ID("InfoSidebarInner"), {
        .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()},
                    .padding = {.top = 16 * s, .right = 16 * s, .bottom = 16 * s, .left = 0},
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .childGap = 16 * s},
        .backgroundColor = {30, 29, 36, 255}
    }) {
        Clay_String infoBoxId = CLAY_STRING("InfoSidebarBox");
        switch (state.sidebarState) {
        case SIDEBAR_IMAGE_INFO:
            infoBox(infoBoxId, 1, s, strings[STR_RESOLUTION_TITLE], strings[STR_RESOLUTION]);
            infoBox(infoBoxId, 2, s, strings[STR_FILESIZE_TITLE], strings[STR_FILESIZE]);
            infoBox(infoBoxId, 3, s, strings[STR_TIME_MODIFIED_TITLE], strings[STR_TIME_MODIFIED]);
            break;
        case SIDEBAR_SETTINGS:
            CLAY(CLAY_ID("SizeSelectorBox"), {
                .layout = { .sizing = { CLAY_SIZING_GROW(), CLAY_SIZING_FIT() },
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                            .padding = CLAY_PADDING_ALL(10 * s),
                            .childGap = 10 * s },
                // .border = { .width = CLAY_BORDER_ALL(2),
                //             .color = C_GRAY },
                .backgroundColor = C_SLATE,
            }) {
                CLAY_TEXT(CLAY_STRING("UI-Skalierung"),
                          { .textColor = C_LIGHTGRAY, .fontSize = 28 * s });
                CLAY(CLAY_ID("SizeSelectorBoxInner"), {
                    .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_FIT()} }
                }) {
                    CLAY_TEXT(fmtClayString(frameArena, "%.2f", state.scale),
                            { .textColor = C_WHITE, .fontId = 1, .fontSize = 32 * s });
                    CLAY(CLAY_ID("SizeSelectorBalloon"), {
                        .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()} } });
                    icon(frameArena, "UiGrowButton", ICON_ADD, C_LIGHTGRAY, C_WHITE, (Clay_Color){0},
                         noTransition, 32 * s);
                    icon(frameArena, "UiShrinkButton", ICON_REMOVE, C_LIGHTGRAY, C_WHITE, (Clay_Color){0},
                         noTransition, 32 * s);
                }
            }
            break;
        default:
            break;
        }
    }
}

Clay_RenderCommandArray createUI(const ApplicationState state, const InputInfo inputs, Arena* frameArena) {
    Clay_SetLayoutDimensions((Clay_Dimensions){ GetScreenWidth(), GetScreenHeight() });
    Clay_SetPointerState(convVec2RaylibClay(inputs.mousePos), inputs.mouseLeftPressed);
    Clay_UpdateScrollContainers(true, convVec2RaylibClay(inputs.mouseScroll), inputs.deltaTime);

    float s = state.scale;
    Clay_BeginLayout();
    CLAY(CLAY_ID("Global"), {
        .layout = { .sizing = {CLAY_SIZING_GROW(), CLAY_SIZING_GROW()} },
        .backgroundColor = C_SLATE
    }) {
        CustomLayoutElement* mainImageData = arena_alloc(frameArena, sizeof(CustomLayoutElement));
        *mainImageData = (CustomLayoutElement){
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
            .custom = { .customData = mainImageData },
        }) {
            float overlayAlpha = state.isImageUiVisible ? 255 : 0;
            Clay_TransitionElementConfig overlayTransition = {
                .handler = Clay_EaseOut,
                .properties = CLAY_TRANSITION_PROPERTY_BACKGROUND_COLOR | CLAY_TRANSITION_PROPERTY_BORDER_COLOR,
                .duration = state.isImageUiVisible ? 0.1f : 0.8f
            };

            int navButtonSize = 48 * s;
            int r = 15 * s;
            Clay_CornerRadius navButtonRadius = {r, r, r, r};
            Clay_Color fg_def = CHANGE_ALPHA(C_LIGHTGRAY, overlayAlpha);
            Clay_Color fg_hov = CHANGE_ALPHA(C_WHITE, overlayAlpha);
            CLAY(CLAY_ID("NavButtonLeft"), {
                .layout = { .sizing = {CLAY_SIZING_FIXED(navButtonSize), CLAY_SIZING_FIXED(navButtonSize)},
                            .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}},
                .floating = { .attachTo = CLAY_ATTACH_TO_PARENT,
                              .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                              .attachPoints = { .element = CLAY_ATTACH_POINT_LEFT_CENTER,
                                                .parent = CLAY_ATTACH_POINT_LEFT_CENTER},
                                                .offset.x = 32 * s },
                .cornerRadius = navButtonRadius,
                .border = { .width = CLAY_BORDER_ALL(3 * s),
                            .color = CHANGE_ALPHA(C_LIGHTGRAY, overlayAlpha) },
                .backgroundColor = Clay_Hovered() ?
                    (Clay_Color){80, 80, 80, overlayAlpha} :
                    CHANGE_ALPHA(C_SLATE, min(overlayAlpha, 230)),
                .transition = overlayTransition
            }) {
                icon(frameArena, "NavButtonLeftIcon", ICON_ARROW_LEFT,
                     fg_def, fg_hov, (Clay_Color){0}, overlayTransition, navButtonSize);
                // CLAY_TEXT(CLAY_STRING("<"),
                //     { .textColor = CHANGE_ALPHA(C_LIGHTGRAY, overlayAlpha), .fontId = 0, .fontSize = 28 });
            }
            CLAY(CLAY_ID("NavButtonRight"), {
                .layout = { .sizing = {CLAY_SIZING_FIXED(navButtonSize), CLAY_SIZING_FIXED(navButtonSize)},
                            .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}},
                .floating = { .attachTo = CLAY_ATTACH_TO_PARENT,
                              .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                              .attachPoints = { .element = CLAY_ATTACH_POINT_RIGHT_CENTER,
                                                .parent = CLAY_ATTACH_POINT_RIGHT_CENTER},
                                                .offset.x = -32 * s },
                .cornerRadius = navButtonRadius,
                .border = { .width = CLAY_BORDER_ALL(3 * s),
                            .color = CHANGE_ALPHA(C_LIGHTGRAY, overlayAlpha) },
                .backgroundColor = Clay_Hovered() ?
                    (Clay_Color){80, 80, 80, overlayAlpha} :
                    CHANGE_ALPHA(C_SLATE, min(overlayAlpha, 230)),
                .transition = overlayTransition
            }) {
                Clay_Color fg_def = CHANGE_ALPHA(C_LIGHTGRAY, overlayAlpha);
                icon(frameArena, "NavButtonRightIcon", ICON_ARROW_RIGHT,
                     fg_def, fg_hov, (Clay_Color){0}, overlayTransition, navButtonSize);
                // CLAY_TEXT(CLAY_STRING(">"),
                //     { .textColor = CHANGE_ALPHA(C_LIGHTGRAY, overlayAlpha), .fontId = 0, .fontSize = 28 });
            }

            bool infoOpen = state.sidebarState == SIDEBAR_IMAGE_INFO;
            CLAY(CLAY_ID("InfoSidebarButton"), {
                .layout = { .sizing = {CLAY_SIZING_FIT(), CLAY_SIZING_FIT()},
                            .padding = CLAY_PADDING_ALL(8 * s)},
                .floating = { .attachTo = CLAY_ATTACH_TO_PARENT,
                              .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                              .attachPoints = { .element = CLAY_ATTACH_POINT_RIGHT_TOP,
                                                .parent = CLAY_ATTACH_POINT_RIGHT_TOP},
                                                .offset = {-16 * s, 16 * s} },
                .cornerRadius = navButtonRadius,
                .border = { .width = CLAY_BORDER_OUTSIDE(3 * s),
                            .color = infoOpen ?
                                        CHANGE_ALPHA(C_WHITE, overlayAlpha) :
                                        CHANGE_ALPHA(C_GRAY, overlayAlpha)},
                .backgroundColor = Clay_Hovered() ?
                    (Clay_Color){80, 80, 80, overlayAlpha} :
                    CHANGE_ALPHA(C_SLATE, overlayAlpha),
                .transition = overlayTransition
            }) {
                // CLAY_TEXT(state.sidebarState == SIDEBAR_IMAGE_INFO ? mkClayString("[i]nfo >"): mkClayString("[i]nfo <"),
                //     { .textColor = CHANGE_ALPHA(C_LIGHTGRAY, overlayAlpha), .fontId = 0, .fontSize = 28 * s });
                Clay_Color infoFg = infoOpen ?
                                        CHANGE_ALPHA(C_WHITE, overlayAlpha) :
                                        CHANGE_ALPHA(C_LIGHTGRAY, overlayAlpha);
                icon(frameArena, "infoIcon", ICON_INFO, infoFg, infoFg, (Clay_Color){}, overlayTransition, 40 * s);
                icon(frameArena, "infoOpenIcon", infoOpen ? ICON_CHEVRON_RIGHT : ICON_CHEVRON_LEFT,
                     infoFg, infoFg, (Clay_Color){}, overlayTransition, 40 * s);
            }
            bool settingsOpen = state.sidebarState == SIDEBAR_SETTINGS;
            CLAY(CLAY_ID("SettingsSidebarButton"), {
                .layout = { .sizing = {CLAY_SIZING_FIT(), CLAY_SIZING_FIT()},
                            .padding = CLAY_PADDING_ALL(8 * s)},
                .floating = { .attachTo = CLAY_ATTACH_TO_PARENT,
                              .pointerCaptureMode = CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH,
                              .attachPoints = { .element = CLAY_ATTACH_POINT_RIGHT_BOTTOM,
                                                .parent = CLAY_ATTACH_POINT_RIGHT_BOTTOM},
                                                .offset = {-16 * s, -16 * s} },
                .cornerRadius = navButtonRadius,
                .border = { .width = CLAY_BORDER_OUTSIDE(3 * s),
                            .color = settingsOpen ?
                                        CHANGE_ALPHA(C_WHITE, overlayAlpha) :
                                        CHANGE_ALPHA(C_GRAY, overlayAlpha)},
                .backgroundColor = Clay_Hovered() ?
                    (Clay_Color){80, 80, 80, overlayAlpha} :
                    CHANGE_ALPHA(C_SLATE, overlayAlpha),
                .transition = overlayTransition
            }) {
                Clay_Color settingsFg = settingsOpen ?
                                            CHANGE_ALPHA(C_WHITE, overlayAlpha) :
                                            CHANGE_ALPHA(C_LIGHTGRAY, overlayAlpha);
                icon(frameArena, "settingsIcon", ICON_SETTINGS, settingsFg, settingsFg, (Clay_Color){}, overlayTransition, 40 * s);
                icon(frameArena, "settingsOpenIcon", state.sidebarState == SIDEBAR_SETTINGS ? ICON_CHEVRON_RIGHT : ICON_CHEVRON_LEFT,
                     settingsFg, settingsFg, (Clay_Color){}, overlayTransition, 40 * s);
            }
        }

        CLAY(CLAY_ID("InfoSidebar"), {
            .layout = { .sizing = {CLAY_SIZING_FIXED(state.sidebarState != SIDEBAR_NONE ? state.sidebarWidth : 0), CLAY_SIZING_GROW()},
                        .padding = {3, 0, 0, 0} },
            .transition = { .handler = Clay_EaseOut,
                            .duration = state.pointerDraggingState == POINTER_DRAGGING_SIDEBAR ? 0.f : .3f,
                            .properties = CLAY_TRANSITION_PROPERTY_WIDTH }
        }) {
            if (state.sidebarState != SIDEBAR_NONE) {
                sidebar(state, inputs, frameArena);
            }
        }
    }
    return Clay_EndLayout(inputs.deltaTime);
}
