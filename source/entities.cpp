#include "entities.hpp"
#include "SRU/assets.hpp"
#include "SRU/render.hpp"
#include "SRU/util.hpp"
#include "events.hpp"
#include <vector>

static std::vector<Entity> entities;
static std::vector<size_t> freeSlots;

// Player
void initPlayer(Entity &player) {
   player.eclass = CLASS_PLAYER;
   player.size = V2(50.0f, 50.0f);
   player.velocity = V2();
   player.direction = V2();
   player.acceleration = 1500.0f;
   player.deceleration = 2000.0f;
   player.speed = 250.0f;
   player.maxHealth = 100.0f;
   player.health = player.maxHealth;
   player.contactDamage = 0.0f;
   player.contactDamageCooldown = 1.0f;
   player.contactDamageTimer = 0.0f;

   Vector2 center = player.position + getOrigin(player.size);
   for (int i = 0; i < playerShadowCount; ++i) {
      player.shadowCenters[i] = center;
      player.shadowSizes[i] = player.size - V2((i + 1) * (player.size.x / 10.0f));
      player.shadowAlphas[i] = 1.0f - (i + 1) * playerShadowAlphaUnit - playerShadowAlphaUnit * 0.5f;
   }
}

void onPlayerDied(Entity &player) {
   player.died = true;
}

void updatePlayer(Entity &player, float DT) {
   float directionX = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
   float directionY = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
   player.direction = Vector2Normalize({directionX, directionY});

   Vector2 targetVelocity = player.direction * player.speed;
   float rate = (directionX != 0.0f || directionY != 0.0f) ? player.acceleration : player.deceleration;
   player.velocity = Vector2MoveTowards(player.velocity, targetVelocity, rate * DT);
   player.position += player.velocity * DT;

   // Update shadows
   Vector2 center = player.position + getOrigin(player.size);
   if (Vector2DistanceSqr(player.shadowCenters[playerShadowCount - 1], center) > 1.0f) {
      for (int i = 0; i < playerShadowCount; ++i) {
         player.shadowCenters[i] = Vector2Lerp(player.shadowCenters[i], center, 0.0458f * (playerShadowCount - i));
      }
   }
   player.bounds = R4(player.position, player.size);
}

void renderPlayer(Entity &player) {
   for (int i = 0; i < playerShadowCount; ++i) {
      drawTextureCentered(getTexture("player"), player.shadowCenters[i], player.shadowSizes[i], Fade(WHITE, player.shadowAlphas[i]));
   }
   drawTexture(getTexture("player"), player.position, player.size);
}

// Floating demon
void initFloatingDemon(Entity &demon) {
   demon.eclass = CLASS_ENEMY;
   demon.size = V2(50.0f, 50.0f);
   demon.velocity = V2();
   demon.direction = V2();
   demon.speed = 125.0f;
   demon.maxHealth = 50.0f;
   demon.health = demon.maxHealth;
   demon.contactDamage = 5.0f;
   demon.contactDamageCooldown = 1.0f;
   demon.contactDamageTimer = 0.0f;
}

void onFloatingDemonDied(Entity &demon) {
   demon.died = true;
}

void updateFloatingDemon(Entity &demon, float DT) {
   demon.position += V2(cos(GetTime()), sin(GetTime()));
   demon.bounds = R4(demon.position, demon.size);
}

void renderFloatingDemon(Entity &demon) {
   drawTexture(getTexture("floating_demon"), demon.position, demon.size);
}

// Entity
void initEntity(Entity &entity, EntityType type, Vector2 position) {
   entity.type = type;
   entity.position = position;
   entity.died = false;

   switch (entity.type) {
   case PLAYER:         initPlayer(entity); break;
   case FLOATING_DEMON: initFloatingDemon(entity); break;
   }
}

void onEntityDied(Entity &entity) {
   switch (entity.type) {
   case PLAYER:         onPlayerDied(entity); break;
   case FLOATING_DEMON: onFloatingDemonDied(entity); break;
   }
}

void updateEntity(Entity &entity, float DT) {
   entity.contactDamageTimer += DT;

   switch (entity.type) {
   case PLAYER:         updatePlayer(entity, DT); break;
   case FLOATING_DEMON: updateFloatingDemon(entity, DT); break;
   }
}

void renderEntity(Entity &entity) {
   switch (entity.type) {
   case PLAYER:         renderPlayer(entity); break;
   case FLOATING_DEMON: renderFloatingDemon(entity); break;
   }
}

// Generic entity functions
void dealContactDamage(Entity &entity, size_t target) {
   if (entity.contactDamage != 0.0f && entity.contactDamageTimer >= entity.contactDamageCooldown) {
      entity.contactDamageTimer = 0.0f;
      pushDamageEvent(target, entity.contactDamage);
   }
}

// Entity handler
size_t spawnEntity(EntityType type, Vector2 position) {
   Entity entity;
   initEntity(entity, type, position);

   if (freeSlots.empty()) {
      entity.ID = entities.size();
      entities.push_back(entity);
   }
   else {
      entity.ID = freeSlots.back();
      freeSlots.pop_back();
      entities[entity.ID] = entity;
   }
   return entity.ID;
}

Entity &getEntity(size_t ID) {
   return entities[ID];
}

void initEntities() {
   entities.clear();
   entities.push_back({.ID = 0});
}

void updateEntities(float DT) {
   for (Entity &entity: entities) {
      if (entity.ID != 0) {
         updateEntity(entity, DT);
      }
   }

   // Resolve collisions
   for (Entity &entity1: entities) {
      for (Entity &entity2: entities) {
         if (entity1.ID == 0 || entity2.ID == 0 || entity1.ID == entity2.ID || entity1.eclass == entity2.eclass) {
            continue;
         }

         if (!CheckCollisionRecs(entity1.bounds, entity2.bounds)) {
            continue;
         }

         bool player = (entity1.eclass == CLASS_PLAYER || entity2.eclass == CLASS_PLAYER);
         bool enemy  = (entity1.eclass == CLASS_ENEMY  || entity2.eclass == CLASS_ENEMY);

         if (player && enemy) {
            dealContactDamage(entity1, entity2.ID);
            dealContactDamage(entity2, entity1.ID);
         }
      }
   }

   // Handle events
   for (HealEvent &event: healEvents) {
      if (event.ID == 0) continue;
      Entity &entity = entities[event.ID];
      entity.health = fminf(entity.maxHealth, entity.health + event.heal);
   }

   for (DamageEvent &event: damageEvents) {
      if (event.ID == 0) continue;
      Entity &entity = entities[event.ID];
      entity.health -= event.damage;

      if (entity.health <= 0.0f) {
         onEntityDied(entity);
      }
   }

   // Cleanup dead enemies
   for (Entity &entity: entities) {
      if (entity.died && entity.ID != 0) {
         freeSlots.push_back(entity.ID);
         entity.ID = 0;
      }
   }
}

void renderEntities() {
   for (Entity &entity: entities) {
      if (entity.ID != 0) {
         renderEntity(entity);
      }
   }
}
