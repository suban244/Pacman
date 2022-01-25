#pragma once
#define GRID_WIDTH 19
#define GRID_HEIGHT 19
#include <SDL2/SDL.h>

enum Direction {
  DirectionRight = 0,
  DirectionDown = 90,
  DirectionLeft = 180,
  DirectionUp = 270

};

class Pacman;
struct Node {
  static int MAX_EDGES;
  int state;
  Node *edges[4];

  Node(int state = 0);
  void addNode(Node *edgeNode);
};

struct EntityLocation {
  short blockX, blockY; // GRID_HEIGHT * GRID_WIDTH
  short offsetX,
      offsetY; // 0 - 5, once you cross the offset, you get to new block
  static int offsetMax;

  EntityLocation(int startX, int startY);
  bool move(bool positiveDirection, bool XDirection);
  bool move(Direction direction);
  void calculateCoordinateToRender(SDL_Rect &dest, int startPosX, int startPosY,
                                   int blockWidth);
  bool atCenter();
};

class Grid {
  static int baseGrid[GRID_HEIGHT][GRID_WIDTH];
  Node nodes[GRID_HEIGHT][GRID_WIDTH];
  int count;

public:
  Grid();
  static bool areConnected(Node *n1, Node *n2);
  Node *getNode(int i, int j);
  bool consume(int i, int j);
  bool complete();
  friend class Pacman;
};
