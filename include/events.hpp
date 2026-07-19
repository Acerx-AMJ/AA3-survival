#pragma once
#include <vector>

struct DamageEvent {
   size_t ID;
   float damage;
};

struct HealEvent {
   size_t ID;
   float heal;
};

void pushDamageEvent(size_t ID, float damage);
void pushHealEvent(size_t ID, float heal);

extern std::vector<DamageEvent> damageEvents;
extern std::vector<HealEvent> healEvents;

void clearEvents();
