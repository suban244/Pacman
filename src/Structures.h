#pragma once
#define GRID_WIDTH 20
#define GRID_HEIGHT 25
#include <SDL2/SDL.h>
#include <ctime>

enum Direction {
  DirectionRight = 0,
  DirectionDown = 90,
  DirectionLeft = 180,
  DirectionUp = 270

};

enum NodeState {
  NodeStateWall = 0,
  NodeStateEmpty = 1,
  NodeStatePoint = 2,
  NodeStatePower = 3
};

enum State {
  StateStarting,
  StatePlaying,
  StatePause,
  StateWon,
  StateLost,
  StateJustDied
};

struct MapInfo {};

class Pacman;

struct Node {
  static int MAX_EDGES;
  NodeState state;
  Node *edges[4];
  int i, j;

  /*
   * @param i the row of the node
   * @param j the column of the node
   * @param state the state of the node
   */
  Node(int i = 0, int j = 0, NodeState state = NodeStateWall);
  void addNode(Node *edgeNode);
};

struct EntityLocation {
  short blockX; // j
  short blockY; // i
  short offsetX,
      offsetY; // 0 - 5, once you cross the offset, you get to new block
  static int offsetMax;

  EntityLocation(int startX, int startY);
  /*
   * @param positiveDirection set to true if we need to move down or right
   * @param XDirection set to true if you have to move either left or right
   * Moves the Entity towards a direction
   */
  bool move(bool positiveDirection, bool XDirection);
  /*
   * @param directon the direction to move in
   * Moves the Entity towards a direction
   */
  bool move(Direction direction);

  /*
   * By using the current direction, determines where to render the entity
   * @param dest Reference to SDL_Rect using for rendreing the entity
   * @param startPosX The x value of the starting pixel of the grid
   * @param startPosY The y value of the starting pixel of the grid
   * @param blockWidth the width of each block in grid
   */
  void calculateCoordinateToRender(SDL_Rect &dest, int startPosX, int startPosY,
                                   int blockWidth);
  /*
   * returns true if entity is at the center of a block
   */
  bool atCenter() const;
};

struct NodeWithParent {
  NodeWithParent *parent;
  Node *baseNode;
  NodeWithParent(Node *baseNode, NodeWithParent *parent);
};

class Grid {
  static int baseGrid[GRID_HEIGHT][GRID_WIDTH];
  Node nodes[GRID_HEIGHT][GRID_WIDTH];
  int baseCount, currentCount;

public:
  int startPosX, startPosY, BLOCK_SIZE;
  Grid();

  /*
   * Resets the grid to original state
   */
  void reset();

  /*
   * @param n1 A reference node
   * @param n2 A neighbouring node
   * Returns true if n1 and n2 are connected
   */
  static bool areConnected(const Node *n1, const Node *n2);

  /*
   * @param i The row of the gird
   * @param j The column of the grid
   * returns a pointer to the node on the grid
   */
  Node *getNode(int i, int j);

  /*
   * @param i The row of the gird
   * @param j The column of the grid
   * Pacman does his consuming thing here, returns true if there was something
   * to consume
   */
  NodeState consume(int i, int j);

  /*
   * Returns true if all the consumable's of the grid are consumed
   */
  bool complete();

  /*
   * @param pacmanLocation Entity's current location
   * @param gameGrid the gird to be referened
   * @param directon The direction to move
   * @param checkOffSet check if entity is at the center of the block
   * (set it to true to return false whenever entity is not at the center of the
   * block)
   * Returns true if entity can move from its currrent location on the
   * grid towards the direciton
   */
  static bool canMove(const EntityLocation pacmanLocation, Grid &gameGrid,
                      Direction direction, bool checkOffSet = false);
  /*
   * @param n1 A reference node
   * @param n2 A neighbouring node
   * Returns the direction of n2 from n1
   * if n2 and n1 are not neighbours returns a random direction
   */
  static Direction FindDirection(const Node *n1, const Node *n2);

  bool isValidGridLocation(EntityLocation location) const {
    return nodes[location.blockY][location.blockX].state != 0;
  }
  bool isValidGridLocation(int i, int j) const {
    return nodes[i][j].state != 0;
  }

  EntityLocation getRandomGridLocation() const;

  friend class Pacman;
};
