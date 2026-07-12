#include "menuState.hpp"
#include <raylib.h>

constexpr int minWindowWidth = 800;
constexpr int minWindowHeight = 600;

int main() {
   SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
   InitWindow(minWindowWidth, minWindowHeight, "AA3-Survival");
   SetWindowMinSize(minWindowWidth, minWindowHeight);
   InitAudioDevice();
   SetExitKey(KEY_NULL);
   SetTraceLogLevel(LOG_ERROR);

   State *current = new MenuState();

   while (!WindowShouldClose()) {
      if (current->shouldChangeState()) {
         State *newState = current->change();
         delete current;
         current = newState;
      }

      if (!current) {
         break;
      }

      current->updateStateLogic();
      current->renderState();
   }

   CloseWindow();
   CloseAudioDevice();
}
