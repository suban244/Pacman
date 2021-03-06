#include "Structures.h"
#include "Enemy.h"
#include "Game.h"
#include <iostream>

int Node::MAX_EDGES = 4;
Node::Node(int i, int j, NodeState state) : state(state), i(i), j(j) {
  // Put this in a initializer list maybe
  edges[0] = edges[1] = edges[2] = edges[3] = nullptr;
}

void Node::addNode(Node *edgeNode) {
  int freeIndex = -1;
  int i = 0;
  for (; i < MAX_EDGES; i++) {
    if (edges[i] == nullptr) {
      freeIndex = i;
      break;
    }
  }
  if (freeIndex == -1) {
    std::cout << " NO Free node";
  } else {
    edges[freeIndex] = edgeNode;
    int j = 0;
    int freeIndex2 = -1;
    for (; j < MAX_EDGES; j++) {
      if (edgeNode->edges[j] == nullptr) {
        freeIndex2 = j;
        break;
      }
    }
    if (freeIndex2 != -1) {
      edgeNode->edges[freeIndex2] = this;
    }
  }
}

NodeWithParent::NodeWithParent(Node *baseNode, NodeWithParent *parent)
    : parent(parent), baseNode(baseNode) {}
/*
 * TODO: Easy
 * Create a Good looking map
 * 0: wall, 1: empty, 2: small write thing, 3: big white thing
 */

int Grid::baseGrid[GRID_HEIGHT][GRID_WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
    {0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0},
    {0, 2, 0, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 0, 2, 0},
    {0, 2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0},
    {0, 2, 2, 2, 2, 2, 0, 0, 2, 1, 1, 2, 0, 0, 2, 2, 2, 2, 2, 0},
    {0, 2, 0, 2, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0},
    {0, 2, 0, 3, 2, 2, 0, 2, 2, 2, 2, 2, 2, 0, 2, 2, 3, 0, 2, 0},
    {0, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 2, 0},
    {0, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 0},
    {0, 0, 2, 2, 2, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 2, 2, 2, 0, 0},
    {0, 2, 2, 0, 0, 2, 2, 2, 0, 0, 1, 0, 2, 2, 2, 0, 0, 2, 2, 0},
    {0, 0, 3, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 3, 0, 0},
    {0, 2, 2, 0, 0, 2, 2, 2, 0, 1, 0, 0, 2, 2, 2, 0, 0, 2, 2, 0},
    {0, 0, 2, 2, 2, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 2, 2, 2, 0, 0},
    {0, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 0},
    {0, 2, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0, 0, 0, 2, 0},
    {0, 2, 0, 3, 2, 2, 0, 2, 2, 2, 2, 2, 2, 0, 2, 2, 3, 0, 2, 0},
    {0, 2, 0, 2, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0},
    {0, 2, 2, 2, 2, 2, 0, 0, 2, 1, 1, 2, 0, 0, 2, 2, 2, 2, 2, 0},
    {0, 2, 0, 2, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 2, 0, 2, 0},
    {0, 2, 0, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 0, 2, 0},
    {0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0},
    {0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

Grid::Grid() {
  baseCount = 0;
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      nodes[i][j] = Node(i, j, NodeState(baseGrid[i][j]));
      if (baseGrid[i][j] != 0) {
        if (baseGrid[i][j] != 1)
          baseCount++;

        if (i != 0)
          if (nodes[i - 1][j].state != 0)
            nodes[i][j].addNode(&nodes[i - 1][j]);
        if (j != 0)
          if (nodes[i][j - 1].state != 0)
            nodes[i][j].addNode(&nodes[i][j - 1]);
      }
    }
  }
  currentCount = baseCount;
  std::srand(std::time(nullptr));

  BLOCK_SIZE = ENTITY_SIZE * 2;
  startPosX = WINDOW_WIDTH / 10 * 3;
  startPosY = 0;
}

void Grid::reset() {
  for (int i = 0; i < GRID_HEIGHT; i++)
    for (int j = 0; j < GRID_WIDTH; j++)
      nodes[i][j].state = NodeState(baseGrid[i][j]);
  currentCount = baseCount;
}

bool Grid::areConnected(const Node *n1, const Node *n2) {
  for (int i = 0; i < Node::MAX_EDGES; i++) {
    if (n1->edges[i] == nullptr)
      return false;
    if (n1->edges[i] == n2)
      return true;
  }
  return false;
}

Node *Grid::getNode(int i, int j) {
  if (i >= GRID_HEIGHT || j >= GRID_WIDTH || i < 0 || j < 0) {
    return nullptr;
  }
  return &nodes[i][j];
}
bool Grid::complete() { return currentCount == 0; }

NodeState Grid::consume(int i, int j) {
  if (i >= GRID_HEIGHT || j >= GRID_WIDTH || i < 0 || j < 0) {
    return NodeStateEmpty;
  }
  if (nodes[i][j].state == NodeStateWall)
    std::cout << "Oh no" << std::endl;
  NodeState state = nodes[i][j].state;
  nodes[i][j].state = NodeStateEmpty;
  if (state > NodeStateEmpty)
    currentCount--;
  return state;
}

Direction Grid::FindDirection(const Node *n1, const Node *n2) {
  if (n1->i == n2->i) {
    if (n2->j > n1->j)
      return DirectionRight;
    else
      return DirectionLeft;
  } else if (n1->j == n2->j) {
    if (n2->i > n1->i)
      return DirectionDown;
    else
      return DirectionUp;
  }
  std::cout << "Nodes not adjuscent: " << n1->i << ", " << n1->j << n2->i
            << ", " << n2->j << std::endl;
  return DirectionDown;
}

EntityLocation::EntityLocation(int startX, int startY)
    : blockX(startX), blockY(startY), offsetX(0), offsetY(0) {}

int EntityLocation::offsetMax = 12; // 0 1 2 3 4
bool EntityLocation::move(bool positiveDirection, bool XDirection) {
  /*
   * return true if we have a block change
   */
  bool blockChange = false;

  if (XDirection) {
    offsetX += positiveDirection ? 1 : -1;
    if (offsetX == offsetMax) {
      blockX++;
      blockChange = true;
      offsetX = (offsetMax - 1) * -1;
    } else if (offsetX == -offsetMax) {
      blockX--;
      blockChange = true;
      offsetX = offsetMax - 1;
    }
  } else {
    offsetY += positiveDirection ? 1 : -1;
    if (offsetY == offsetMax) {
      blockY++;
      blockChange = true;
      offsetY = (offsetMax - 1) * -1;
    } else if (offsetY == -offsetMax) {
      blockY--;
      blockChange = true;
      offsetY = offsetMax - 1;
    }
  }
  return blockChange;
}

bool EntityLocation::move(Direction direction) {
  bool XDirection = (direction == DirectionLeft || direction == DirectionRight)
                        ? true
                        : false;
  bool positiveDirection =
      (direction == DirectionDown || direction == DirectionRight) ? true
                                                                  : false;
  return move(positiveDirection, XDirection);
}

bool EntityLocation::atCenter() const { return (offsetX == 0 && offsetY == 0); }

void EntityLocation::calculateCoordinateToRender(SDL_Rect &dest, int startPosX,
                                                 int startPosY,
                                                 int blockWidth) {
  int margin = (blockWidth - dest.w) / 2;
  dest.x = startPosX + blockX * blockWidth + margin +
           offsetX * blockWidth / offsetMax / 2;
  dest.y = startPosY + blockY * blockWidth + margin +
           offsetY * blockWidth / offsetMax / 2;
}
bool Grid::canMove(const EntityLocation pacmanLocation, Grid &gameGrid,
                   Direction direction, bool checkOffSet) {
  if (direction == DirectionDown || direction == DirectionUp) {
    if (pacmanLocation.offsetX == 0 || !checkOffSet) {
      // It can move / change direction vertically
      if (pacmanLocation.offsetY != 0)
        return true;
      if (Grid::areConnected(
              gameGrid.getNode(pacmanLocation.blockY, pacmanLocation.blockX),
              gameGrid.getNode(pacmanLocation.blockY +
                                   ((direction == DirectionDown) ? 1 : -1),
                               pacmanLocation.blockX)))
        return true;
    }
  } else {
    if (pacmanLocation.offsetY == 0 || !checkOffSet) {
      // It can move / change direction horizontally
      if (pacmanLocation.offsetX != 0)
        return true;
      if (Grid::areConnected(
              gameGrid.getNode(pacmanLocation.blockY, pacmanLocation.blockX),
              gameGrid.getNode(pacmanLocation.blockY,
                               pacmanLocation.blockX +
                                   ((direction == DirectionRight) ? 1 : -1))))

        return true;
    }
  }
  return false;
}

EntityLocation Grid::getRandomGridLocation() const {
  int i, j;
  while (true) {
    i = std::rand() / ((RAND_MAX + 1u) / GRID_HEIGHT);
    j = std::rand() / ((RAND_MAX + 1u) / GRID_WIDTH);
    if (isValidGridLocation(i, j))
      return EntityLocation(j, i);
  }
}
