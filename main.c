// #define GLSL_VERSION 460

#include "raylib.h"
#include <stdio.h>

static inline float min(float a, float b) {return a < b ? a : b;}
static inline float max(float a, float b) {return a > b ? a : b;}

int main() {
   const int screenWidth  = 800;
   const int screenHeight = 600;
   InitWindow(screenWidth, screenHeight, "guten Morgen");
   Shader helloShader = LoadShader(0, "shaders/hello.frag");
   Shader imageShader = LoadShader(0, "./shaders/image.frag");

   RenderTexture2D fullscreenRect =
       LoadRenderTexture(screenWidth - 100, screenHeight);
   Texture2D image = LoadTexture("./res/saft.png");
   RenderTexture2D imageFrameBuf =
       LoadRenderTexture(screenWidth - 100, screenHeight);

   printf("image dims: %d x %d\n", image.width, image.height);

   SetTargetFPS(24);

   while (!WindowShouldClose()) {
      BeginDrawing();
      {
         ClearBackground(RAYWHITE);
         BeginShaderMode(imageShader);
         {
            // NOTE: Render texture must be y-flipped due to default OpenGL
            // coordinates (left-bottom)

            // DrawTextureRec(fullscreenRect.texture,
            //                (Rectangle){0, 0,
            //                (float)fullscreenRect.texture.width,
            //                            (float)-fullscreenRect.texture.height},
            //                (Vector2){0, 0}, BLACK);

            // int rectDimsLoc    = GetShaderLocation(imageShader, "rectDims");
            // int pictureDimsLoc = GetShaderLocation(imageShader, "pictureDims");
            // float rectDims[]     = {screenWidth - 100, screenHeight};
            // float pictureDims[]  = {image.width, image.height};
            int picScaleLoc = GetShaderLocation(imageShader, "picScale");
            float imgScale = max(
               (float)image.width / (screenWidth - 100.f),
               (float)image.height / screenHeight
            );
            // printf("%f %f\n", imgScale[0], imgScale[1]);
            SetShaderValue(
               imageShader, picScaleLoc, &imgScale, SHADER_UNIFORM_FLOAT
            );
            DrawTextureRec(
               image,
               (Rectangle){0, 0, (float)screenWidth - 100, (float)screenHeight},
               (Vector2){0, 0},
               BLACK
            );
         }
         EndShaderMode();
      }
      EndDrawing();
   }
   CloseWindow();
   return 0;
}
