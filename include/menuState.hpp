#pragma once
#include "raylib.h"
#include "state.hpp"
#include <string>

struct MenuState: public State {
   MenuState();
   ~MenuState() = default;

   State *change() override;

   void updateResponsiveness() override;
   void update() override;
   void render() override;

private:

   void updateLoadingState();
   void renderLoadingState();

   enum class Load {fonts, textures, shaders, sounds, data, done};
   std::string loadingText;
   Load load = Load::fonts;
   float loadingTimer = 0.0f;
   float iconRotation = 0.0f;

   enum class Phase {title};
   Phase phase = Phase::title;
   bool settings = false;
   bool quitting = false;

   Rectangle playButton, optionsButton, quitButton; 
};
