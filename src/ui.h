#ifndef INCLUDE_SRC_UI_H_
#define INCLUDE_SRC_UI_H_

#include "lib/arena.h"
#include "lib/clay.h"
#include "state.h"

Clay_RenderCommandArray createUI(const ApplicationState state, const InputInfo inputs, Arena* frameArena);

#endif  // INCLUDE_SRC_UI_H_
