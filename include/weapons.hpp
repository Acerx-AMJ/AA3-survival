#pragma once
#include "raylib.h"
#include <cstddef>

enum WeaponType {
   PISTOL
};

struct Weapon {
   WeaponType type;
   
   Vector2 size;
   Vector2 origin;
   float angle;
   float firerate;
   float timeSinceLastShot;
};

void initWeapon(Weapon &weapon, WeaponType type, size_t player);
void updateWeapon(Weapon &weapon, float DT, size_t player);
void renderWeapon(Weapon &weapon, size_t player);
