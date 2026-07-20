#pragma once
#include "raylib.h"
#include <cstddef>

static constexpr int playerShadowCount = 3;
static constexpr float playerShadowAlphaUnit = 1.0f / (playerShadowCount + 1);

enum EntityClass {
   CLASS_PLAYER,
   CLASS_ENEMY,
   CLASS_PLAYER_PROJECTILE,
   CLASS_ENEMY_PROJECTILE
};

enum EntityType {
   PLAYER,
   FLOATING_DEMON,
   SHOOTING_DEMON,
   PISTOL_PROJECTILE,
   DEMON_PROJECTILE,
};

struct Entity {
   size_t ID;
   EntityClass eclass;
   EntityType type;
   Vector2 position;

   Rectangle bounds;
   Vector2 size;
   Vector2 velocity;
   Vector2 direction;
   float angle;
   float acceleration;
   float deceleration;
   float speed;
   float maxHealth;
   float health;

   float contactDamage;
   float contactDamageCooldown;
   float contactDamageTimer;
   float aliveTime;
   float despawnTime;
   bool died;

   float firerate;
   float timeSincelastShot;

   Vector2 shadowCenters[playerShadowCount];
   Vector2 shadowSizes[playerShadowCount];
   float shadowAlphas[playerShadowCount];
};

void initEntity(Entity &entity, EntityType type, Vector2 position);
void onEntityDied(Entity &entity);
void updateEntity(Entity &entity, float DT, size_t player);
void renderEntity(Entity &entity);

size_t spawnEntity(EntityType type, Vector2 position);
Entity &getEntity(size_t ID);

void initEntities();
void updateEntities(float DT, size_t player);
void renderEntities();
