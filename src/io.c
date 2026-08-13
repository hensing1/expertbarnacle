#include "io.h"

InputInfo captureInputs() {
    return (InputInfo){
        .deltaTime = GetFrameTime(),
        .mousePos = GetMousePosition(),
        .mouseDelta = GetMouseDelta(),
        .mouseScroll = GetMouseWheelMoveV(),
        .mouseLeftPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT),
        .mouseRightPressed = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT),
        .mouseLeftDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT),
        .mouseRightDown = IsMouseButtonDown(MOUSE_BUTTON_RIGHT)
    };
}
