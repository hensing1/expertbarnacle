#ifndef INCLUDE_SRC_TEXTURES_H_
#define INCLUDE_SRC_TEXTURES_H_

#include <raylib.h>

typedef enum {
    ICON_ADD,
    ICON_REMOVE,
    ICON_RESET,
    ICON_CHEVRON_LEFT,
    ICON_CHEVRON_RIGHT,
    NUM_ICONS
} IconID;

void initIcons();
Texture2D* getIcon(IconID icon);

#endif  // INCLUDE_SRC_TEXTURES_H_
