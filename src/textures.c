#include "textures.h"

#include <stdlib.h>

static Texture2D* icons;

void initIcons() {
    icons = malloc(sizeof(Texture2D) * NUM_ICONS);
    icons[ICON_ADD]           = LoadTexture("./res/icons/ci--add-plus-square.png");
    icons[ICON_REMOVE]        = LoadTexture("./res/icons/ci--add-minus-square.png");
    icons[ICON_RESET]         = LoadTexture("./res/icons/ci--arrow-undo-up-left.png");
    icons[ICON_ARROW_LEFT]    = LoadTexture("./res/icons/ci--arrow-left-sm.png");
    icons[ICON_ARROW_RIGHT]   = LoadTexture("./res/icons/ci--arrow-right-sm.png");
    icons[ICON_CHEVRON_LEFT]  = LoadTexture("./res/icons/ci--chevron-left-md.png");
    icons[ICON_CHEVRON_RIGHT] = LoadTexture("./res/icons/ci--chevron-right-md.png");
    icons[ICON_INFO]          = LoadTexture("./res/icons/ci--info.png");
    icons[ICON_SETTINGS]      = LoadTexture("./res/icons/ci--settings.png");
}

Texture2D* getIcon(IconID icon) {
    return icons + icon;
}
