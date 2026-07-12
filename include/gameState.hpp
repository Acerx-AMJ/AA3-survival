#pragma once
#include "state.hpp"

struct GameState: public State {
   GameState();
   ~GameState() = default;

   State *change() override;

   void update() override;
   void render() override;

private:

   
};
