#include "Enemy.h"

Enemy::Enemy(int i, int j) : location(j, i) {
  destRect.w = destRect.h = ENTITY_SIZE;
}
void Enemy::init(int startPosX, int startPosY, int blockWidth) {
  this->startPosX = startPosX;
  this->startPosY = startPosY;
  this->blockWidth = blockWidth;
  location.calculateCoordinateToRender(destRect, startPosX, startPosY,
                                       blockWidth);
  sprite.loadFromFile("assets/enemy.png");
}

/*
 * Moves the Enemy object in a certain direction
 * Then updates where we shound render it
 */
void Enemy::update() {
  // TODO: Move the enemy object
  location.calculateCoordinateToRender(destRect, startPosX, startPosY,
                                       blockWidth);
}
void Enemy::render() { sprite.render(&destRect); }

void Enemy::DFS_search(Grid &gameGrid) {
  /*
   * TODO: Hard
   *
   */
}

bool Enemy::detectColision(SDL_Rect &enemyRect) {
  /*
   * TODO: Easy
   * Detect colision between enemyRect and destRect
   * return true if colision, otherwise false
   * SDL_Rect has 4 attributes w(width), h(height), x, y(top corner of the
   * rectangle) Figure out if the rectangle overlap, if they do, return true
   * else false
   */
  return false;
}
