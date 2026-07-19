#include "entities.hpp"
#include "SRU/assets.hpp"
#include "SRU/render.hpp"
#include "SRU/util.hpp"
#include <vector>

static std::vector<Entity> entities;

// Player
void initPlayer(Entity &player) {
   player.size = V2(50.0f, 50.0f);
   player.velocity = V2();
   player.direction = V2();
   player.acceleration = 1500.0f;
   player.deceleration = 2000.0f;
   player.speed = 250.0f;
   player.maxHealth = 100.0f;
   player.health = player.maxHealth;

   Vector2 center = player.position + getOrigin(player.size);
   for (int i = 0; i < playerShadowCount; ++i) {
      player.shadowCenters[i] = center;
      player.shadowSizes[i] = player.size - V2((i + 1) * (player.size.x / 10.0f));
      player.shadowAlphas[i] = 1.0f - (i + 1) * playerShadowAlphaUnit - playerShadowAlphaUnit * 0.5f;
   }
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
}

void renderPlayer(Entity &player) {
   for (int i = 0; i < playerShadowCount; ++i) {
      drawTextureCentered(getTexture("player"), player.shadowCenters[i], player.shadowSizes[i], Fade(WHITE, player.shadowAlphas[i]));
   }
   drawTexture(getTexture("player"), player.position, player.size);
}

// Floating demon
void initFloatingDemon(Entity &demon) {
   demon.size = V2(50.0f, 50.0f);
   demon.velocity = V2();
   demon.direction = V2();
   demon.speed = 125.0f;
   demon.health = 50.0f;
}

void updateFloatingDemon(Entity &demon, float DT) {
   demon.position += V2(cos(GetTime()), sin(GetTime()));
}

void renderFloatingDemon(Entity &demon) {
   drawTexture(getTexture("floating_demon"), demon.position, demon.size);
}

// Entity
void initEntity(Entity &entity, EntityType type, Vector2 position) {
   entity.type = type;
   entity.position = position;

   switch (entity.type) {
   case PLAYER:         initPlayer(entity); break;
   case FLOATING_DEMON: initFloatingDemon(entity); break;
   }
}

void updateEntity(Entity &entity, float DT) {
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

// Entity handler
size_t spawnEntity(EntityType type, Vector2 position) {
   Entity entity;
   initEntity(entity, type, position);
   entities.push_back(entity);
   return entities.size() - 1;
}

Entity &getEntity(size_t ID) {
   return entities[ID];
}

void initEntities() {
   entities.clear();
}

void updateEntities(float DT) {
   for (Entity &entity: entities) {
      updateEntity(entity, DT);
   }
}

void renderEntities() {
   for (Entity &entity: entities) {
      renderEntity(entity);
   }
}
