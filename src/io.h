#ifndef INCLUDE_SRC_IO_H_
#define INCLUDE_SRC_IO_H_

#include <raylib.h>

typedef struct {
    float deltaTime;
    Vector2 mousePos;
    Vector2 mouseDelta;
    Vector2 mouseScroll;
    bool mouseLeftPressed;
    bool mouseRightPressed;
    bool mouseLeftDown;
    bool mouseRightDown;
} InputInfo;

InputInfo captureInputs();

#endif  // INCLUDE_SRC_IO_H_
