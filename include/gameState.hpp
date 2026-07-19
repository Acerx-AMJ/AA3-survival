#pragma once
#include "state.hpp"
#include "weapons.hpp"
#include <cstddef>

struct GameState: public State {
   GameState();
   ~GameState() = default;

   State *change() override;

   void update() override;
   void render() override;

private:

   size_t player;
   Weapon weapon;
};
