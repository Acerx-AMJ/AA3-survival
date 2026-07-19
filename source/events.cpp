#include "events.hpp"

std::vector<DamageEvent> damageEvents;
std::vector<HealEvent> healEvents;

void pushDamageEvent(size_t ID, float damage) {
   damageEvents.push_back(DamageEvent{ID, damage});
}

void pushHealEvent(size_t ID, float damage) {
   healEvents.push_back(HealEvent{ID, damage});
}

void clearEvents() {
   damageEvents.clear();
   healEvents.clear();
}
