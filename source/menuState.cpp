#include "menuState.hpp"
#include "entities.hpp"
#include "gameState.hpp"
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
      load = Load::done;
   }
   else {
      entities.reserve(512);
      freeSlots.reserve(512);
   }
   updateResponsiveness();
}

State *MenuState::change() {
   if (settings) {
      return nullptr; // while no settings
   }
   else if (quitting) {
      return nullptr; // quit the game
   }
   return new GameState();
}

void MenuState::updateResponsiveness() {
   playButton = mapCubicAreaCentered(V2(0.5f, 0.5f), V2(0.1f, 0.1f));
   optionsButton = mapCubicAreaCentered(V2(0.33f, 0.5f), V2(0.1f, 0.1f));
   quitButton = mapCubicAreaCentered(V2(0.66f, 0.5f), V2(0.1f, 0.1f));
}

void MenuState::update() {
   if (!loadedAssets) {
      updateLoadingState();
      return;
   }

   Vector2 mouse = GetMousePosition();
   bool clicked = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);

   if (clicked && CheckCollisionPointRec(mouse, playButton)) {
      quitState();
   }

   if (clicked && CheckCollisionPointRec(mouse, optionsButton)) {
      settings = true;
      quitState();
   }

   if (clicked && CheckCollisionPointRec(mouse, quitButton)) {
      quitting = true;
      quitState();
   }
}

void MenuState::render() {
   if (load == Load::done) {
      drawTextCenteredResponsive(getFont("slackey"), V2(0.5f, 0.1f), "AA3-Survival", 80.0f);
      drawTexture(getTexture("play"), R4pos(playButton), R4size(playButton));
      drawTexture(getTexture("cogwheel"), R4pos(optionsButton), R4size(optionsButton));
      drawTexture(getTexture("quit"), R4pos(quitButton), R4size(quitButton));
   }

   if (!loadedAssets) {
      renderLoadingState();
   }
}

void MenuState::updateLoadingState() {
   iconRotation += DT * iconRotationSpeed;

   if (load == Load::fonts) {
      loadFonts("assets/fonts");
      loadingText = "Loading Textures...";
      load = Load::textures;
   }
   else if (load == Load::textures) {
      loadTextures("assets/textures");
      loadingText = "Loading Shaders...";
      load = Load::shaders;
   }
   else if (load == Load::shaders) {
      loadShaders("assets/shaders");
      loadingText = "Loading Sounds...";
      load = Load::sounds;
   }
   else if (load == Load::sounds) {
      loadSounds("assets/sounds");
      loadingText = "Loading Done!";
      load = Load::done;
   }
   else if (load == Load::done) {
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
