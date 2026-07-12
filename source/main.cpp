#include "SRU/sru.hpp"
#include <raylib.h>

int main() {
   InitWindow(800, 600, "AA3-Survival");
   SetTargetFPS(60);

   while (!WindowShouldClose()) {
      BeginDrawing();
         ClearBackground(BLACK);
         DrawCircleV(getWindowCenter(), sin(GetTime()) * 12.5f * 4.0f + 37.5f * 4.0f, HSV(fmodf(GetTime() * 120.0f, 360.0f), 1.0f, 1.0f));
      EndDrawing();
   }
}