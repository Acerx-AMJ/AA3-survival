#include "player.hpp"
#include "SRU/assets.hpp"
#include "SRU/render.hpp"
#include "SRU/util.hpp"

void Player::init() {
   Vector2 center = position + getOrigin(size);
   for (int i = 0; i < shadowCount; ++i) {
      shadows[i].center = center;
      shadows[i].size = size - V2((i + 1) * (size.x / 10.0f));
      shadows[i].alpha = 1.0f - (i + 1) * shadowAlphaUnit - shadowAlphaUnit * 0.5f;
   }
}

void Player::update(float DT) {
   float directionX = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
   float directionY = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
   direction = Vector2Normalize({directionX, directionY});

   Vector2 targetVelocity = direction * speed;
   float rate = (directionX != 0.0f || directionY != 0.0f) ? acceleration : deceleration;
   velocity = Vector2MoveTowards(velocity, targetVelocity, rate * DT);
   position += velocity * DT;

   // Update shadows
   Vector2 center = position + getOrigin(size);
   if (Vector2DistanceSqr(shadows[shadowCount-1].center, center) > 1.0f) {
      for (int i = 0; i < shadowCount; ++i) {
         shadows[i].center = Vector2Lerp(shadows[i].center, center, 0.0458f * (shadowCount - i));
      }
   }
}

void Player::render() {
   for (int i = 0; i < shadowCount; ++i) {
      drawTextureCentered(getTexture("player"), shadows[i].center, shadows[i].size, Fade(WHITE, shadows[i].alpha));
   }
   drawTexture(getTexture("player"), position, size);
}
