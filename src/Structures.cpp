#include "Structures.h"
#include <iostream>

int Node::MAX_EDGES = 4;
Node::Node(int state) : state(state) {
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

int Grid::baseGrid[GRID_HEIGHT][GRID_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    //{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

Grid::Grid() {
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      nodes[i][j] = Node(baseGrid[i][j]);
      if (baseGrid[i][j] != 0) {
        if (i != 0)
          if (nodes[i - 1][j].state != 0)
            nodes[i][j].addNode(&nodes[i - 1][j]);
        if (j != 0)
          if (nodes[i][j - 1].state != 0)
            nodes[i][j].addNode(&nodes[i][j - 1]);
      }
    }
  }
}

bool Grid::areConnected(Node *n1, Node *n2) {
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

EntityLocation::EntityLocation(int startX, int startY)
    : blockX(startX), blockY(startY), offsetX(0), offsetY(0) {}

int EntityLocation::offsetMax = 5; // 0 1 2 3 4
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

bool EntityLocation::atCenter() { return (offsetX == 0 && offsetY == 0); }

void EntityLocation::calculateCoordinateToRender(SDL_Rect &dest, int startPosX,
                                                 int startPosY,
                                                 int blockWidth) {
  int margin = (blockWidth - dest.w) / 2;
  dest.x = startPosX + blockX * blockWidth + margin +
           offsetX * blockWidth / offsetMax / 2;
  dest.y = startPosY + blockY * blockWidth + margin +
           offsetY * blockWidth / offsetMax / 2;
}
