#include <stdlib.h>

#include "raylib.h"

int main(void)
{
    InitWindow(800, 600, "test");
    malloc(500);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        EndDrawing();
    }

    CloseWindow();
}
