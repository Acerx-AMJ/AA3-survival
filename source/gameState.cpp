#include "gameState.hpp"
#include "entities.hpp"

GameState::GameState() {
   initEntities();
   spawnEntity(PLAYER, {0.0f, 0.0f});
}

State *GameState::change() {
   return nullptr;
}

void GameState::update() {
   updateEntities(DT);
}

void GameState::render() {
   renderEntities();
}
