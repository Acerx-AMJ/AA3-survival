#pragma once
#include "raylib.h"
#include <cstddef>

static constexpr int playerShadowCount = 3;
static constexpr float playerShadowAlphaUnit = 1.0f / (playerShadowCount + 1);

enum EntityType {
   PLAYER, FLOATING_DEMON
};

struct Entity {
   EntityType type;
   Vector2 position;

   Vector2 size;
   Vector2 velocity;
   Vector2 direction;
   float acceleration;
   float deceleration;
   float speed;
   float maxHealth;
   float health;

   Vector2 shadowCenters[playerShadowCount];
   Vector2 shadowSizes[playerShadowCount];
   float shadowAlphas[playerShadowCount];
};

void initEntity(Entity &entity, EntityType type, Vector2 position);
void updateEntity(Entity &entity, float DT);
void renderEntity(Entity &entity);

size_t spawnEntity(EntityType type, Vector2 position);
Entity &getEntity(size_t ID);

void initEntities();
void updateEntities(float DT);
void renderEntities();
