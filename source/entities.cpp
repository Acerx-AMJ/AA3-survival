#include "entities.hpp"
#include "player.hpp"

static Player player;

void initEntities() {
   player = {};
   player.init();
}

void updateEntities(float DT) {
   player.update(DT);
}

void renderEntities() {
   player.render();
}
