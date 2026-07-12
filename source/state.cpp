#include "state.hpp"
#include "raylib.h"
#include <cmath>

void State::updateStateLogic() {
   int width = GetScreenWidth();
   int height = GetScreenHeight();

   if (width != lastWidth || height != lastHeight) {
      lastWidth = width;
      lastHeight = height;
      updateResponsiveness();
   }

   if (fadingIn) {
      updateFadingIn();
      update();
   }
   else if (fadingOut) {
      updateFadingOut();
   }
   else {
      update();
   }

   realDT = GetFrameTime();
   DT = fminf(realDT, maxDT);

   accumulator += DT;
   while (accumulator >= fixedUpdateDT) {
      fixedUpdate();
      accumulator -= fixedUpdateDT;
   }
}

void State::renderState() {
   BeginDrawing();
      ClearBackground(BLACK);
      render();
      DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, alpha));
   EndDrawing();
}

void State::quitState() {
   fadingOut = true;
}

bool State::shouldChangeState() const {
   return quittingState;
}

void State::updateFadingIn() {
   fadeTimer += realDT;
   alpha = 1.0f - (fadeTimer / fadeTime);

   if (fadeTimer >= fadeTime) {
      fadeTimer = 0.0f;
      alpha = 0.0f;
      fadingIn = false;
   }
}

void State::updateFadingOut() {
   fadeTimer += realDT;
   alpha = fadeTimer / fadeTime;

   if (fadeTimer >= fadeTime) {
      alpha = 1.0f;
      fadingOut = false;
      quittingState = true;
   }
}
