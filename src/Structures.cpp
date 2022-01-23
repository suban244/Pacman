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
    edges[i] = edgeNode;
  }
}

int Grid::baseGrid[GRID_HEIGHT][GRID_WIDTH] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

Grid::Grid() {
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {
      nodes[i][j] = Node(baseGrid[i][j]);
      if (baseGrid[i][j] == 0) {
      }
    }
  }
}

EntityLocation::EntityLocation(int startX, int startY)
    : blockX(startX), blockY(startY), offsetX(0), offsetY(0) {}

void EntityLocation::move(bool positiveDirection, bool XDirection) {
  if (XDirection) {
    offsetX = positiveDirection ? 1 : -1;
    if (offsetX == 5) {
      blockX++;
    } else if (offsetX == -5) {
      blockX--;
    }
  } else {
    offsetY = positiveDirection ? 1 : -1;
    if (offsetY == 5) {
      blockX++;
    } else if (offsetY == -5) {
      blockX--;
    }
  }
}

void EntityLocation::calculateCoordinateToRender(SDL_Rect &dest, int startPosX,
                                                 int startPosY,
                                                 int blockWidth) {
  int margin = (blockWidth - dest.w) / 2;
  dest.x = startPosX + blockX * blockWidth + margin;
  dest.y = startPosY + blockY * blockWidth + margin;
}
