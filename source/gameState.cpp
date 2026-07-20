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
      
      Entity &playerEntity = getEntity(player);
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
   }
   else {
      DrawText("You died! Press r to restart.\n Or esc to go to main menu.", 20, 20, 80, RED);
   }
}
