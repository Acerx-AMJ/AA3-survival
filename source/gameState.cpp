#include "gameState.hpp"
#include "entities.hpp"
#include "events.hpp"

GameState::GameState() {
   initEntities();
   spawnEntity(PLAYER, {0.0f, 0.0f});
   spawnEntity(FLOATING_DEMON, {500.0f, 500.0f});
   spawnEntity(FLOATING_DEMON, {230.0f, 450.0f});
   spawnEntity(FLOATING_DEMON, {800.0f, 600.0f});
}

State *GameState::change() {
   return nullptr;
}

void GameState::update() {
   clearEvents();
   updateEntities(DT);
}

void GameState::render() {
   renderEntities();
}
