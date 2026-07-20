#include "SRU/assets.hpp"
#include "SRU/render.hpp"
#include "SRU/text.hpp"
#include "SRU/util.hpp" // IWYU pragma: export
#include "gameState.hpp"
#include "entities.hpp"
#include "events.hpp"
#include "menuState.hpp"

GameState::GameState() {
   initEntities();
   player = spawnEntity(PLAYER, {0.0f, 0.0f});

   spawnEntity(FLOATING_DEMON, {500.0f, 500.0f});
   spawnEntity(FLOATING_DEMON, {230.0f, 450.0f});
   spawnEntity(FLOATING_DEMON, {800.0f, 600.0f});
   spawnEntity(SHOOTING_DEMON, {1200.0f, 800.0f});
   spawnEntity(SHOOTING_DEMON, {1600.0f, 100.0f});
   initWeapon(weapon, PISTOL, player);
}

State *GameState::change() {
   if (restart) {
      return new GameState();
   }
   return new MenuState();
}

void GameState::update() {
   clearEvents();

   if (!playerdied) {
      updateWeapon(weapon, DT, player);
      updateEntities(DT, player);
      
      Entity &playerEntity = entities[player];
      if (playerEntity.died || playerEntity.ID == 0) {
         playerdied = true;
      }
   }

   if (IsKeyPressed(KEY_R)) {
      restart = true;
      quitState();
   }

   if (IsKeyPressed(KEY_ESCAPE)) {
      quitState();
   }
}

void GameState::render() {
   renderEntities();

   if (!playerdied) {
      renderWeapon(weapon, player);
      Entity &playerEntity = entities[player];

      Vector2 healthbarCenter = mapRatioToScreen(0.5f, 0.1f);
      Vector2 healthbarSize = mapCubicRatioToScreen(0.3f, 0.05f);
      float ratio = playerEntity.health / playerEntity.maxHealth;

      drawRect(healthbarCenter - healthbarSize / 2.0f, healthbarSize, RED);
      drawRect(healthbarCenter - healthbarSize / 2.0f, V2(healthbarSize.x * ratio, healthbarSize.y), GREEN);

      Font font = getFont("slackey");

      drawTextCentered(font, healthbarCenter, TextFormat("%.1f/%.1f", playerEntity.health, playerEntity.maxHealth), getFontSizeScaled(30.0f), BLACK);
      drawText(font, V2(10.0f, 10.0f), TextFormat("EC: %llu/%llu", entities.size(), entities.capacity()), getFontSizeScaled(20.0f));
      drawText(font, V2(10.0f, 40.0f), TextFormat("FS: %llu/%llu", freeSlots.size(), freeSlots.capacity()), getFontSizeScaled(20.0f));
      drawText(font, V2(10.0f, 70.0f), TextFormat("AE: %llu", entities.size() - freeSlots.size()), getFontSizeScaled(20.0f));
   }
   else {
      DrawText("You died! Press r to restart.\n Or esc to go to main menu.", 20, 20, 80, RED);
   }
}
