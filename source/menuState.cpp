#include "menuState.hpp"
#include "SRU/assets.hpp"
#include "SRU/util.hpp"
#include "SRU/render.hpp"

constexpr float iconRotationSpeed = 360.0f; // degrees per second
constexpr float loadingTime = 1.0f;
static bool loadedAssets = false; // one-time procedure

MenuState::MenuState() {
   loadingText = "Loading Fonts...";
   loadFont("slackey", "assets/fonts/slackey.ttf");
   loadTexture("loading", "assets/textures/loading.png");
   if (loadedAssets) {
      phase = Load::done;
   }
}

State *MenuState::change() {
   return nullptr;
}

void MenuState::update() {
   if (!loadedAssets) {
      updateLoadingState();
      return;
   }
}

void MenuState::render() {
   if (phase != Load::done) {
      renderLoadingState();
      return;
   }

   drawRectResponsive(V2(0.0f, 0.0f), V2(1.0f, 1.0f), BLUE);

   if (!loadedAssets) {
      renderLoadingState();
   }
}

void MenuState::updateLoadingState() {
   iconRotation += DT * iconRotationSpeed;

   if (phase == Load::fonts) {
      loadFonts("assets/fonts");
      loadingText = "Loading Textures...";
      phase = Load::textures;
   }
   else if (phase == Load::textures) {
      loadTextures("assets/textures");
      loadingText = "Loading Shaders...";
      phase = Load::shaders;
   }
   else if (phase == Load::shaders) {
      loadShaders("assets/shaders");
      loadingText = "Loading Sounds...";
      phase = Load::sounds;
   }
   else if (phase == Load::sounds) {
      loadSounds("assets/sounds");
      loadingText = "Loading Done!";
      phase = Load::done;
   }
   else if (phase == Load::done) {
      loadingTimer += DT;
      if (loadingTimer >= loadingTime) {
         loadedAssets = true;
      }
   }
}

void MenuState::renderLoadingState() {
   if (loadedAssets) {
      return;
   }
   
   float progress = loadingTimer / loadingTime;

   drawRectResponsive(V2(0.0f, progress), V2(1.0f, 1.0f), BLACK);
   drawTextureCenteredResponsiveCubic(getTexture("loading"), V2(0.5f, 0.5f + progress), V2(0.1f, 0.1f));
   drawTextCenteredResponsive(getFont("slackey"), V2(0.5f, 0.375f + progress), loadingText.c_str(), 60.0f);   
}
