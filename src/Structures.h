#define GRID_WIDTH 19
#define GRID_HEIGHT 19
#include <SDL2/SDL.h>

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

  EntityLocation(int startX, int startY);
  void move(bool positiveDirection, bool XDirection);
  void calculateCoordinateToRender(SDL_Rect &dest, int startPosX, int startPosY,
                                   int blockWidth);
};

class Grid {
  static int baseGrid[GRID_HEIGHT][GRID_WIDTH];
  Node nodes[GRID_HEIGHT][GRID_WIDTH];

public:
  Grid();
  friend class Pacman;
};
