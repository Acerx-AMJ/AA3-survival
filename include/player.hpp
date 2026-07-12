#pragma once
#include "raylib.h"

struct Player {
   void init();
   void update(float DT);
   void render();

   Vector2 size {50.0f, 50.0f};
   Vector2 position = {0.0f, 0.0f};
   Vector2 velocity;
   Vector2 direction;
   float acceleration = 1500.0f;
   float deceleration = 2000.0f;
   float speed = 250.0f;
   float maxHealth = 100.0f;
   float health = maxHealth;

private:
   static constexpr int shadowCount = 3;
   static constexpr float shadowAlphaUnit = 1.0f / (shadowCount + 1);

   struct Shadow {
      Vector2 center, size;
      float alpha;
   };
   Shadow shadows[3];
};
