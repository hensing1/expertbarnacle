#include <raylib.h>

#include <stdio.h>

int main() {
  InitWindow(1000, 800, "hi");
  SetTargetFPS(60);
  while(!WindowShouldClose()) {
    BeginDrawing();
      // ...
    EndDrawing();
  }
  return 0;
}
