#include "entities.hpp"
#include "SRU/assets.hpp"
#include "SRU/render.hpp"
#include "SRU/util.hpp"
#include "events.hpp"
#include <vector>

static std::vector<Entity> entities;
static std::vector<size_t> freeSlots;
static Vector2 lastPlayerPosition = V2();

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
   player.despawnTime = 0.0f;

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
   demon.speed = 100.0f;
   demon.maxHealth = 50.0f;
   demon.health = demon.maxHealth;
   demon.contactDamage = 5.0f;
   demon.contactDamageCooldown = 1.0f;
   demon.contactDamageTimer = 0.0f;
   demon.despawnTime = 0.0f;
}

void onFloatingDemonDied(Entity &demon) {
   demon.died = true;
}

void updateFloatingDemon(Entity &demon, float DT, Vector2 playerCenter) {
   demon.position += Vector2Normalize(playerCenter - R4center(demon.bounds)) * demon.speed * DT;
   demon.bounds = R4(demon.position, demon.size);
}

void renderFloatingDemon(Entity &demon) {
   drawTexture(getTexture("floating_demon"), demon.position, demon.size);
}

// Shooting demon
void initShootingDemon(Entity &demon) {
   demon.eclass = CLASS_ENEMY;
   demon.size = V2(75.0f, 75.0f);
   demon.velocity = V2();
   demon.direction = V2();
   demon.speed = 75.0f;
   demon.maxHealth = 100.0f;
   demon.health = demon.maxHealth;
   demon.contactDamage = 10.0f;
   demon.contactDamageCooldown = 1.0f;
   demon.contactDamageTimer = 0.0f;
   demon.despawnTime = 0.0f;
   demon.firerate = 60.0f / 100.0f;
   demon.timeSincelastShot = 0.0f;
}

void onShootingDemonDied(Entity &demon) {
   demon.died = true;
}

void updateShootingDemon(Entity &demon, float DT, Vector2 playerCenter) {
   demon.position += V2(cos(GetTime()) * 0.5f, sin(GetTime()) * 0.5f);
   demon.bounds = R4(demon.position, demon.size);

   if (demon.timeSincelastShot >= demon.firerate) {
      demon.timeSincelastShot = 0.0f;

      Vector2 shotPosition = R4center(demon.bounds);
      size_t projectile = spawnEntity(DEMON_PROJECTILE, shotPosition);
      Entity &projectileEntity = getEntity(projectile);
      projectileEntity.direction = Vector2Normalize(playerCenter - shotPosition);
      projectileEntity.angle = atan2(playerCenter.y - shotPosition.y, playerCenter.x - shotPosition.x) * 180.0f / PI;
   }
}

void renderShootingDemon(Entity &demon) {
   drawTexture(getTexture("floating_demon"), demon.position, demon.size);
}

// Pistol projectile
void initPistolProjectile(Entity &projectile) {
   projectile.eclass = CLASS_PLAYER_PROJECTILE;
   projectile.size = V2(20.0f, 20.0f);
   projectile.speed = 400.0f;
   projectile.contactDamage = 10.0f;
   projectile.despawnTime = 2.0f;
}

void onPistolProjectileDied(Entity &projectile) {
   projectile.died = true;
}

void updatePistolProjectile(Entity &projectile, float DT, Vector2 playerCenter) {
   projectile.position += projectile.direction * projectile.speed * DT;
   projectile.bounds = R4(projectile.position, projectile.size);
}

void renderPistolProjectile(Entity &projectile) {
   drawRect(projectile.bounds, YELLOW, projectile.angle);
}

// Demon projectile
void initDemonProjectile(Entity &projectile) {
   projectile.eclass = CLASS_ENEMY_PROJECTILE;
   projectile.size = V2(20.0f, 20.0f);
   projectile.speed = 300.0f;
   projectile.contactDamage = 10.0f;
   projectile.despawnTime = 2.0f;
}

void onDemonProjectileDied(Entity &projectile) {
   projectile.died = true;
}

void updateDemonProjectile(Entity &projectile, float DT, Vector2 playerCenter) {
   projectile.position += projectile.direction * projectile.speed * DT;
   projectile.bounds = R4(projectile.position, projectile.size);
}

void renderDemonProjectile(Entity &projectile) {
   drawRect(projectile.bounds, GREEN, projectile.angle);
}

// Entity
void initEntity(Entity &entity, EntityType type, Vector2 position) {
   entity.type = type;
   entity.position = position;
   entity.aliveTime = 0.0f;
   entity.died = false;

   switch (entity.type) {
   case PLAYER:            initPlayer(entity); break;
   case FLOATING_DEMON:    initFloatingDemon(entity); break;
   case SHOOTING_DEMON:    initShootingDemon(entity); break;
   case PISTOL_PROJECTILE: initPistolProjectile(entity); break;
   case DEMON_PROJECTILE:  initDemonProjectile(entity); break;
   }
}

void onEntityDied(Entity &entity) {
   switch (entity.type) {
   case PLAYER:            onPlayerDied(entity); break;
   case FLOATING_DEMON:    onFloatingDemonDied(entity); break;
   case SHOOTING_DEMON:    onShootingDemonDied(entity); break;
   case PISTOL_PROJECTILE: onPistolProjectileDied(entity); break;
   case DEMON_PROJECTILE:  onDemonProjectileDied(entity); break;
   }
}

void updateEntity(Entity &entity, float DT, size_t player) {
   Entity &playerEntity = entities[player];
   if (playerEntity.ID != 0) {
      lastPlayerPosition = R4center(playerEntity.bounds);
   }

   Vector2 position = lastPlayerPosition;   
   entity.timeSincelastShot += DT;
   entity.contactDamageTimer += DT;
   entity.aliveTime += DT;

   switch (entity.type) {
   case PLAYER:            updatePlayer(entity, DT); break;
   case FLOATING_DEMON:    updateFloatingDemon(entity, DT, position); break;
   case SHOOTING_DEMON:    updateShootingDemon(entity, DT, position); break;
   case PISTOL_PROJECTILE: updatePistolProjectile(entity, DT, position); break;
   case DEMON_PROJECTILE:  updateDemonProjectile(entity, DT, position); break;
   }

   if (entity.despawnTime != 0.0f && entity.aliveTime >= entity.despawnTime) {
      onEntityDied(entity);
   }
}

void renderEntity(Entity &entity) {
   switch (entity.type) {
   case PLAYER:            renderPlayer(entity); break;
   case FLOATING_DEMON:    renderFloatingDemon(entity); break;
   case SHOOTING_DEMON:    renderShootingDemon(entity); break;
   case PISTOL_PROJECTILE: renderPistolProjectile(entity); break;
   case DEMON_PROJECTILE:  renderDemonProjectile(entity); break;
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
   freeSlots.clear();
   entities.clear();
   entities.push_back({.ID = 0});
}

void updateEntities(float DT, size_t player) {
   for (Entity &entity: entities) {
      if (entity.ID != 0) {
         updateEntity(entity, DT, player);
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

         bool player      = (entity1.eclass == CLASS_PLAYER            || entity2.eclass == CLASS_PLAYER);
         bool enemy       = (entity1.eclass == CLASS_ENEMY             || entity2.eclass == CLASS_ENEMY);
         bool pprojectile = (entity1.eclass == CLASS_PLAYER_PROJECTILE || entity2.eclass == CLASS_PLAYER_PROJECTILE);
         bool eprojectile = (entity1.eclass == CLASS_ENEMY_PROJECTILE  || entity2.eclass == CLASS_ENEMY_PROJECTILE);

         if (player && enemy) {
            dealContactDamage(entity1, entity2.ID);
            dealContactDamage(entity2, entity1.ID);
         }
         else if (enemy && pprojectile) {
            Entity &enemyEntity = (entity1.eclass == CLASS_ENEMY ? entity1 : entity2);
            Entity &projectileEntity = (entity1.eclass == CLASS_PLAYER_PROJECTILE ? entity1 : entity2);
            dealContactDamage(projectileEntity, enemyEntity.ID);
            onEntityDied(projectileEntity);
         }
         else if (player && eprojectile) {
            Entity &playerEntity = (entity1.eclass == CLASS_PLAYER ? entity1 : entity2);
            Entity &projectileEntity = (entity1.eclass == CLASS_ENEMY_PROJECTILE ? entity1 : entity2);
            dealContactDamage(projectileEntity, playerEntity.ID);
            onEntityDied(projectileEntity);
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
