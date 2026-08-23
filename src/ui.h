#ifndef INCLUDE_SRC_UI_H_
#define INCLUDE_SRC_UI_H_

#include "lib/clay.h"
#include "render.h"
#include "state.h"

Clay_RenderCommandArray createUI(ApplicationState state, InputInfo inputs, Clay_String* strings);

#endif  // INCLUDE_SRC_UI_H_
