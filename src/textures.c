#include "textures.h"

#include <stdlib.h>

static Texture2D* icons;

void initIcons() {
    icons = malloc(sizeof(Texture2D) * NUM_ICONS);
    icons[ICON_ADD]    = LoadTexture("./res/icons/ci--add-plus-square.png");
    icons[ICON_REMOVE] = LoadTexture("./res/icons/ci--add-minus-square.png");
    icons[ICON_RESET]  = LoadTexture("./res/icons/ci--arrow-undo-up-left.png");
}

Texture2D* getIcon(IconID icon) {
    return icons + icon;
}
