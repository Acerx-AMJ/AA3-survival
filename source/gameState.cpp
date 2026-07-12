#include "gameState.hpp"
#include "entities.hpp"

GameState::GameState() {
   initEntities();
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
