#pragma once
#include "state.hpp"
#include <string>

struct MenuState: public State {
   MenuState();
   ~MenuState() = default;

   State *change() override;

   void update() override;
   void render() override;

private:

   void updateLoadingState();
   void renderLoadingState();

   enum class Load {fonts, textures, shaders, sounds, data, done};
   std::string loadingText;
   Load phase = Load::fonts;
   float loadingTimer = 0.0f;
   float iconRotation = 0.0f;
};
