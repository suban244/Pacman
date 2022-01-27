#pragma once
#include "Structures.h"
#include "Texture.h"
#include <ctime>
#include <stack>
#include <vector>
#define ENTITY_SIZE 20

struct Enemy {
  Texture sprite;
  EntityLocation location;
  SDL_Rect srcRect, destRect;
  Direction direction;
  int animationCount;

  int startPosX, startPosY, blockWidth;

  Enemy(int i, int j);
  void init(int startPosX, int startPosY, int blockWidth);
  bool detectColision(SDL_Rect &enemyRect);

  void update(Grid &gameGrid, EntityLocation &pacmanLocation);
  void render();

private:
  Direction getRandomDirection(Grid &gameGrid);
  void moveFullyRandom(Grid &gameGrid);
  void moveStrainghtRandom(Grid &gameGrid);

  void moveWithDFS(Grid &gameGrid, EntityLocation &pacmanLocation);
  Direction DFS_search(Grid &gameGrid, EntityLocation &pacmanLocation);
};
