#include "Enemy.h"
#include "Structures.h"

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
  std::srand(std::time(nullptr));
}

void Enemy::update(Grid &gameGrid, EntityLocation &pacmanLocation) {
  // TODO: Move the enemy object

  // DFS_search(gameGrid, pacmanLocation);
  location.calculateCoordinateToRender(destRect, startPosX, startPosY,
                                       blockWidth);
  // Random movement
  // moveWithDFS(gameGrid, pacmanLocation);
  moveWithBFS(gameGrid, pacmanLocation);
}

void Enemy::moveStrainghtRandom(Grid &gameGrid) {
  if (Grid::canMove(location, gameGrid, direction)) {
    location.move(direction);
  } else {
    moveFullyRandom(gameGrid);
  }
}

Direction Enemy::getRandomDirection(Grid &gameGrid) {
  int x;
  while (true) {
    x = 1 + std::rand() / ((RAND_MAX + 1u) / 4);
    direction = (x == 1)   ? DirectionUp
                : (x == 2) ? DirectionRight
                : (x == 3) ? DirectionLeft
                           : DirectionDown;
    if (Grid::canMove(location, gameGrid, direction)) {
      return direction;
    }
  }
}

void Enemy::moveFullyRandom(Grid &gameGrid) {
  if (location.atCenter()) {
    location.move(getRandomDirection(gameGrid));
  } else {
    location.move(direction);
  }
}

void Enemy::render() { sprite.render(&destRect); }

bool vectorContainsNode(std::vector<Node *> &vec, Node *node) {
  bool contains = false;
  for (Node *n : vec) {
    if (n == node)
      contains = true;
  }
  return contains;
}

void Enemy::moveWithDFS2(Grid &gameGrid, EntityLocation &pacmanLocation) {
  if (location.atCenter()) {
    if (pathToBeFollowed.empty()) {
      direction = DFS_search(gameGrid, pacmanLocation, pathToBeFollowed);

      if (!pathToBeFollowed.empty())
        pathToBeFollowed.erase(pathToBeFollowed.begin());
      if (!pathToBeFollowed.empty())
        pathToBeFollowed.erase(pathToBeFollowed.begin());
    } else {
      direction = Grid::FindDirection(
          gameGrid.getNode(location.blockY, location.blockX),
          pathToBeFollowed.front());
      pathToBeFollowed.erase(pathToBeFollowed.begin());
    }
  }
  location.move(direction);
}

void Enemy::moveWithDFS(Grid &gameGrid, EntityLocation &pacmanLocation) {
  if (location.atCenter()) {
    std::vector<Node *> pathFollowed;
    direction = DFS_search(gameGrid, pacmanLocation, pathFollowed);
  }
  location.move(direction);
}

void Enemy::moveWithBFS(Grid &gameGrid, EntityLocation &pacmanLocation) {
  if (location.atCenter()) {
    std::vector<Node *> pathFollowed;
    direction = BFS_sarech(gameGrid, pacmanLocation, pathFollowed);
  }
  location.move(direction);
}

Direction Enemy::DFS_search(Grid &gameGrid, EntityLocation &pacmanLocation,
                            std::vector<Node *> &pathFollowed) {
  /*
   * TODO: Hard
   *
   */
  std::stack<Node *> frontier;
  std::vector<Node *> explored;

  Node *startNode = gameGrid.getNode(location.blockY, location.blockX);
  Node *destinationNode =
      gameGrid.getNode(pacmanLocation.blockY, pacmanLocation.blockX);
  frontier.push(startNode);
  Node *temp;

  while (!frontier.empty()) {
    temp = frontier.top();
    pathFollowed.push_back(temp);
    if (temp == destinationNode) {
      // Yay
      break;
    } else {
      frontier.pop();
      explored.push_back(temp);

      bool nodeAdded = false;
      for (int i = 0; i < 4; i++) {
        if (temp->edges[i] == nullptr)
          break;

        // Check if the node is in explored
        bool contains = vectorContainsNode(explored, temp->edges[i]);
        if (!contains) {
          frontier.push(temp->edges[i]);
          nodeAdded = true;
        }
      }
      if (!nodeAdded) {
        // Pop until unexplored node in edge
        pathFollowed.pop_back();
        while (!pathFollowed.empty()) {
          bool allExplored = true;
          for (int i = 0; i < 4; i++) {
            if (pathFollowed.back()->edges[i] == nullptr)
              break;

            bool contains =
                vectorContainsNode(explored, pathFollowed.back()->edges[i]);
            if (!contains) {
              allExplored = false;
            }
          }
          if (allExplored) {
            pathFollowed.pop_back();
          } else {
            break;
          }
        }
      }
    }
  }

  if (pathFollowed.size() > 2) {
    return Grid::FindDirection(startNode, pathFollowed[1]);
  } else {
    return getRandomDirection(gameGrid);
  }
}

Direction Enemy::BFS_sarech(Grid &gameGrid, EntityLocation &pacmanLocation,
                            std::vector<Node *> pathFollowed) {

  std::queue<NodeWithParent *> frontier;
  std::vector<Node *> explored;
  std::vector<NodeWithParent *> addedNodes;

  NodeWithParent *startNode = new NodeWithParent(
      gameGrid.getNode(location.blockY, location.blockX), nullptr);
  addedNodes.push_back(startNode);

  Node *destinationNode =
      gameGrid.getNode(pacmanLocation.blockY, pacmanLocation.blockX);

  frontier.push(startNode);

  NodeWithParent *temp;
  while (!frontier.empty()) {
    temp = frontier.front();
    frontier.pop();
    explored.push_back(temp->baseNode);

    if (temp->baseNode == destinationNode) {
      // YAY
      break;
    } else {

      bool nodeAdded = false;
      for (int i = 0; i < 4; i++) {
        if (temp->baseNode->edges[i] == nullptr)
          break;
        // Check if the node is in explored
        bool contains = vectorContainsNode(explored, temp->baseNode->edges[i]);
        if (!contains) {
          // temp->baseNode->edges[i])
          frontier.push(new NodeWithParent(temp->baseNode->edges[i], temp));
          addedNodes.push_back(frontier.back());
          nodeAdded = true;
        }
      }
      if (!nodeAdded) {
        // Delete the nodewithparent
        addedNodes.erase(
            std::remove(addedNodes.begin(), addedNodes.end(), temp),
            addedNodes.end());

        delete temp;
      }
    }
  }
  while (temp) {
    pathFollowed.insert(pathFollowed.begin(), temp->baseNode);
    temp = temp->parent;
  }
  while (!addedNodes.empty()) {
    delete addedNodes.back();
    addedNodes.pop_back();
  }
  if (pathFollowed.size() > 1) {
    return Grid::FindDirection(pathFollowed[0], pathFollowed[1]);
  } else
    return getRandomDirection(gameGrid);
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
