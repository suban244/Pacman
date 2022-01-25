#pragma once
#include "Structures.h"
#include "Texture.h"
#define ENTITY_SIZE 20

struct Enemy {
  Texture sprite;
  EntityLocation location;
  SDL_Rect srcRect, destRect;
  Direction direction, nextDirection;
  int animationCount;

  int startPosX, startPosY, blockWidth;

  Enemy(int i, int j);
  void init(int startPosX, int startPosY, int blockWidth);
  bool detectColision(SDL_Rect &enemyRect);
  void DFS_search(Grid &gameGrid);

  void update();
  void render();
};
