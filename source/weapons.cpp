#include "weapons.hpp"
#include "SRU/assets.hpp"
#include "SRU/render.hpp"
#include "SRU/util.hpp"
#include "entities.hpp"

// Pistol
void initPistol(Weapon &pistol) {
   pistol.size = V2(20.0f, 20.0f);
   pistol.angle = 0.0f;
   pistol.firerate = 60.0f / 300.0f;
   pistol.timeSinceLastShot = 0.0f;
}

void updatePistol(Weapon &pistol, float DT, Vector2 playerCenter) {
   if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && pistol.timeSinceLastShot >= pistol.firerate) {
      pistol.timeSinceLastShot = 0.0f;

      size_t projectile = spawnEntity(PISTOL_PROJECTILE, playerCenter);
      Entity &projectileEntity = getEntity(projectile);
      projectileEntity.direction = Vector2Normalize(GetMousePosition() - playerCenter);
      projectileEntity.angle = pistol.angle;
   }
}

void renderPistol(Weapon &pistol, Vector2 playerCenter) {
   drawTextureOrigin(getTexture("pistol"), playerCenter, pistol.origin, pistol.size, WHITE, pistol.angle);
}

// Weapon
void initWeapon(Weapon &weapon, WeaponType type, size_t player) {
   Entity &entity = getEntity(player);
   if (entity.ID == 0) return;
   
   weapon.type = type;

   switch (weapon.type) {
   case PISTOL: initPistol(weapon); break;
   }
   weapon.origin = V2(weapon.size.x / 2.0f + entity.size.x, weapon.size.y / 2.0f);
}

void updateWeapon(Weapon &weapon, float DT, size_t player) {
   Entity &entity = getEntity(player);
   if (entity.ID == 0) return;
   Vector2 center = entity.position + entity.size / 2.0f;
   Vector2 mouse = GetMousePosition();

   weapon.angle = atan2(center.y - mouse.y, center.x - mouse.x) * 180.0f / PI;
   weapon.timeSinceLastShot += DT;
   
   switch (weapon.type) {
   case PISTOL: updatePistol(weapon, DT, center); break;
   }
}

void renderWeapon(Weapon &weapon, size_t player) {
   Entity &entity = getEntity(player);
   if (entity.ID == 0) return;
   Vector2 center = entity.position + entity.size / 2.0f;
   
   switch (weapon.type) {
   case PISTOL: renderPistol(weapon, center); break;
   }
}
