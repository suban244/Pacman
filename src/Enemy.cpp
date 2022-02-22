#include "Enemy.h"
#include "Structures.h"

float calcEuclideanDistance(int x1, int y1, int x2, int y2) {
  return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

Enemy::Enemy(Grid &gameGrid, int i, int j, EnemyType e)
    : baseLocation(j, i), location(j, i), type(e) {

  destRect.w = destRect.h = ENTITY_SIZE;

  texture.loadFromFile("assets/enemy.png");
  std::srand(std::time(nullptr));
}

void Enemy::init(Grid &gameGrid) {
  location = EntityLocation(baseLocation);
  location.calculateCoordinateToRender(destRect, gameGrid.startPosX,
                                       gameGrid.startPosY, gameGrid.BLOCK_SIZE);
  direction = getRandomDirection(gameGrid);
  pathToBeFollowed.clear();
  state = EnemyStateChasing;
}

void Enemy::update(Grid &gameGrid, EntityLocation &pacmanLocation) {
  // TODO: Move the enemy object

  // DFS_search(gameGrid, pacmanLocation);
  if (state == EnemyStateChasing) {
    switch (type) {
    case ENEMY_RANDOM:
      moveFullyRandom(gameGrid);
      break;
    case ENEMY_RANDOM_STRAIGHT:
      moveStrainghtRandom(gameGrid);
      break;

    case ENEMY_DFS_BAD:
      moveWithDFS(gameGrid, pacmanLocation);
      break;

    case ENEMY_DFS_LESS_BAD:
      moveWithDFS2(gameGrid, pacmanLocation);
      break;

    case ENEMY_BFS:
      moveWithBFS(gameGrid, pacmanLocation);
      break;
    case ENEMY_EUCLIDEAN:
      moveWithEuclideanDistance(gameGrid, pacmanLocation);
      break;
    case ENEMY_ASTAR:
      moveWithAStar(gameGrid, pacmanLocation);
      break;
    default:
      moveFullyRandom(gameGrid);
    }
  } else if (state == EnemyStateRunning) {
    // runWithEculideanDistance(gameGrid, pacmanLocation);
    runningTimer--;
    if (runningTimer == 0) {
      state = EnemyStateChasing;
      pathToBeFollowed.clear();
    }
    moveStrainghtRandom(gameGrid);
  } else {
    moveWithBFS(gameGrid, baseLocation);
    if (inBaseLocation()) {
      state = EnemyStateChasing;
      pathToBeFollowed.clear();
    } else {
      moveWithBFS(gameGrid, baseLocation);
      if (inBaseLocation()) {
        state = EnemyStateChasing;
        pathToBeFollowed.clear();
      }
    }
  }

  location.calculateCoordinateToRender(destRect, gameGrid.startPosX,
                                       gameGrid.startPosY, gameGrid.BLOCK_SIZE);
}

bool Enemy::inBaseLocation() {
  if (location.offsetX != 0 || location.offsetY != 0)
    return false;
  if (location.blockX == baseLocation.blockX &&
      location.blockY == baseLocation.blockY)
    return true;
  else
    return false;
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

void Enemy::render(const Grid &gameGrid, Texture &enemyRunningSprite,
                   Texture &enemyResetingSprite, Uint8 alpha) {
  switch (state) {
  case EnemyStateChasing:
    texture.setAlpha(alpha);
    texture.render(&destRect);
    break;
  case EnemyStateRunning:
    if (runningTimer < ENEMY_RUNNING_TIMER_MAX / 10) {
      if ((runningTimer / 6) % 2 == 0) {
        texture.setAlpha(alpha);
        texture.render(&destRect);
      } else {
        enemyRunningSprite.setAlpha(alpha);
        enemyRunningSprite.render(&destRect);
      }
    } else {
      enemyRunningSprite.setAlpha(alpha);
      enemyRunningSprite.render(&destRect);
    }
    break;
  case EnemyStateReseting:
    enemyResetingSprite.setAlpha(alpha);
    enemyResetingSprite.render(&destRect);
    break;
  }
}

int vectorContainsNode(std::vector<Node *> &vec, Node *node) {
  if (node == nullptr)
    return -1;

  size_t i = 0;
  for (; i < vec.size(); i++) {
    if (vec[i] == node)
      return i;
  }
  return -1;
}
int vectorContainsBaseNode(std::vector<NodeWithParent *> &vec, Node *node) {
  size_t i = 0;
  for (; i < vec.size(); i++) {
    if (vec[i]->baseNode == node)
      return i;
  }
  return -1;
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

void Enemy::moveWithAStar(Grid &gameGrid, EntityLocation &pacmanLocation) {
  if (location.atCenter()) {
    std::vector<Node *> pathFollowed;
    direction = AStarSearch(gameGrid, pacmanLocation, pathFollowed);
  }
  location.move(direction);
}

Direction Enemy::DFS_search(Grid &gameGrid, EntityLocation &pacmanLocation,
                            std::vector<Node *> &pathFollowed) {
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
        int contains = vectorContainsNode(explored, temp->edges[i]);
        if (contains == -1) {
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

            int contains =
                vectorContainsNode(explored, pathFollowed.back()->edges[i]);
            if (contains == -1) {
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

  if (pathFollowed.size() > 1) {
    return Grid::FindDirection(startNode, pathFollowed[1]);
  } else {
    return getRandomDirection(gameGrid);
  }
}

Direction Enemy::BFS_sarech(Grid &gameGrid, EntityLocation &pacmanLocation,
                            std::vector<Node *> &pathFollowed) {

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
  int nodesExplored = 0;
  while (!frontier.empty()) {
    nodesExplored++;
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
        int contains = vectorContainsNode(explored, temp->baseNode->edges[i]);
        if (contains == -1) {
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

  /*
  std::cout << "BFS Nodes Explored: " << nodesExplored << " "
            << "Path of length:" << pathFollowed.size() << std::endl;
            */

  if (pathFollowed.size() > 1) {
    return Grid::FindDirection(pathFollowed[0], pathFollowed[1]);
  } else
    return getRandomDirection(gameGrid);
}

void Enemy::moveWithEuclideanDistance(Grid &gameGrid,
                                      EntityLocation &pacmanLocation) {
  // Top distance
  if (location.atCenter()) {
    float minDistance = 1000;
    float dist;
    Direction minDirection = DirectionUp;
    if (Grid::canMove(location, gameGrid, DirectionUp)) {
      dist = calcEuclideanDistance(pacmanLocation.blockX, pacmanLocation.blockY,
                                   location.blockX, location.blockY - 1);
      if (dist < minDistance) {
        minDistance = dist;
        minDirection = DirectionUp;
      }
    }
    if (Grid::canMove(location, gameGrid, DirectionDown)) {
      dist = calcEuclideanDistance(pacmanLocation.blockX, pacmanLocation.blockY,
                                   location.blockX, location.blockY + 1);
      if (dist < minDistance) {
        minDistance = dist;
        minDirection = DirectionDown;
      }
    }
    if (Grid::canMove(location, gameGrid, DirectionLeft)) {
      dist = calcEuclideanDistance(pacmanLocation.blockX, pacmanLocation.blockY,
                                   location.blockX - 1, location.blockY);
      if (dist < minDistance) {
        minDistance = dist;
        minDirection = DirectionLeft;
      }
    }
    if (Grid::canMove(location, gameGrid, DirectionRight)) {
      dist = calcEuclideanDistance(pacmanLocation.blockX, pacmanLocation.blockY,
                                   location.blockX + 1, location.blockY);
      if (dist < minDistance) {
        minDistance = dist;
        minDirection = DirectionRight;
      }
    }

    direction = minDirection;
  }
  location.move(direction);
}

void Enemy::runWithEculideanDistance(Grid &gameGrid,
                                     EntityLocation &pacmanLocation) {
  // Top distance
  if (location.atCenter()) {
    float maxDistance = 0;
    float dist;
    Direction minDirection = DirectionUp;
    if (Grid::canMove(location, gameGrid, DirectionUp)) {
      dist = calcEuclideanDistance(pacmanLocation.blockX, pacmanLocation.blockY,
                                   location.blockX, location.blockY - 1);
      if (dist > maxDistance) {
        maxDistance = dist;
        minDirection = DirectionUp;
      }
    }
    if (Grid::canMove(location, gameGrid, DirectionDown)) {
      dist = calcEuclideanDistance(pacmanLocation.blockX, pacmanLocation.blockY,
                                   location.blockX, location.blockY + 1);
      if (dist > maxDistance) {
        maxDistance = dist;
        minDirection = DirectionDown;
      }
    }
    if (Grid::canMove(location, gameGrid, DirectionLeft)) {
      dist = calcEuclideanDistance(pacmanLocation.blockX, pacmanLocation.blockY,
                                   location.blockX - 1, location.blockY);
      if (dist > maxDistance) {
        maxDistance = dist;
        minDirection = DirectionLeft;
      }
    }
    if (Grid::canMove(location, gameGrid, DirectionRight)) {
      dist = calcEuclideanDistance(pacmanLocation.blockX, pacmanLocation.blockY,
                                   location.blockX + 1, location.blockY);
      if (dist > maxDistance) {
        maxDistance = dist;
        minDirection = DirectionRight;
      }
    }

    direction = minDirection;
  }
  location.move(direction);
}
bool cmpDjistra(NodeWithParent *n1, NodeWithParent *n2) {
  float l1 = 0;
  float l2 = 0;

  NodeWithParent *temp;
  for (temp = n1; temp != nullptr; temp = temp->parent)
    l1++;

  for (temp = n2; temp != nullptr; temp = temp->parent)
    l2++;

  return l1 > l2;
};

float calcCost(NodeWithParent *n, EntityLocation pacmanLocation) {
  float cost = 0;
  NodeWithParent *temp = n;

  for (; temp != nullptr; temp = temp->parent)
    cost++;

  /*
  cost *= cost;
  cost += calcEuclideanDistance(n->baseNode->j, n->baseNode->i,
                                pacmanLocation.blockX, pacmanLocation.blockY);
                                */
  cost += abs(n->baseNode->j - pacmanLocation.blockX) +
          abs(n->baseNode->i - pacmanLocation.blockY);
  return cost;
}

Direction Enemy::AStarSearch(Grid &gameGrid, EntityLocation &pacmanLocation,
                             std::vector<Node *> &pathFollowed) {
  /*
   * TODO
   */
  pathFollowed.clear();
  Node *destinationNode =
      gameGrid.getNode(pacmanLocation.blockY, pacmanLocation.blockX);

  std::unordered_map<Node *, Node *> parentMap;
  std::vector<Node *> parentMapKeys; // Vector of all the keys of parent map

  auto reconstructPath = [&pathFollowed, &parentMap, &parentMapKeys](Node *n) {
    pathFollowed.push_back(n);
    while (vectorContainsNode(parentMapKeys, n)) {
      n = parentMap[n];
      if (n == nullptr)
        break;
      pathFollowed.push_back(n);
    }
    n = parentMap[n];
    if (n)
      pathFollowed.push_back(n);
  };

  std::unordered_map<Node *, int> gScoreMap; // Cheapest path fron start to n
  std::unordered_map<Node *, int> fscoreMap; // gscore(n) + h(n)

  auto gScore = [&gScoreMap](Node *n) {
    try {
      return gScoreMap.at(n);
    } catch (std::out_of_range const &) {
      return 100000;
    }
  };
  auto fscore = [&fscoreMap](Node *n) {
    try {
      return fscoreMap.at(n);
    } catch (std::out_of_range const &) {
      return 100000;
    }
  };

  auto h = [pacmanLocation](Node *n) {
    return abs(n->j - pacmanLocation.blockX) +
           abs(n->i - pacmanLocation.blockY);
  };

  /*
  auto calcCost = [&parentMap, h](Node *n, EntityLocation pacmanLocation) {
    float cost = 0;
    Node *temp = n;
    for (; temp != nullptr; temp = parentMap.at(n))
      cost++;
    cost += h(n);
    return cost;
  };
  */

  auto cmp = [fscore](Node *n1, Node *n2) { return fscore(n1) > fscore(n2); };

  std::priority_queue<Node *, std::vector<Node *>, decltype(cmp)> frontier(cmp);

  Node *temp, *neighbor;
  int nodesExplored = 0;
  frontier.push(gameGrid.getNode(location.blockY, location.blockX));

  gScoreMap[frontier.top()] = 0;
  fscoreMap[frontier.top()] = h(frontier.top());

  while (!frontier.empty()) {
    nodesExplored++;
    temp = frontier.top();

    if (temp == destinationNode) {
      // YAY
      // Reconstruct path
      reconstructPath(temp);
      break;
    }

    frontier.pop();

    for (int i = 0; i < 4; i++) {
      neighbor = temp->edges[i];
      if (neighbor == nullptr)
        break;
      float tentative_gScore = gScore(temp) + 1;
      if (tentative_gScore < gScore(neighbor)) {

        parentMap[neighbor] = temp;
        parentMapKeys.push_back(neighbor);

        gScoreMap[neighbor] = tentative_gScore;
        fscoreMap[neighbor] = tentative_gScore + h(neighbor);

        if (vectorContainsNode(parentMapKeys, neighbor) != -1) {
          frontier.push(neighbor);
        }
      }
    }
  }
  /*
  std::cout << "A* Nodes Explored: " << nodesExplored << " "
            << "Path of length:" << pathFollowed.size() << std::endl;
            */

  if (pathFollowed.size() > 1) {
    return Grid::FindDirection(pathFollowed[pathFollowed.size() - 1],
                               pathFollowed[pathFollowed.size() - 2]);
  } else
    return getRandomDirection(gameGrid);
}

void Enemy::reset() {}

bool Enemy::detectColision(SDL_Rect &enemyRect) {
  /*
   * TODO: Easy
   * Detect colision between enemyRect and destRect
   * return true if colision, otherwise false
   * SDL_Rect has 4 attributes w(width), h(height), x, y(top corner of the
   * rectangle) Figure out if the rectangle overlap, if they do, return true
   * else false
   */
  if (enemyRect.x <= destRect.x + destRect.w &&
      enemyRect.x + enemyRect.w >= destRect.x &&
      enemyRect.y <= destRect.y + destRect.h &&
      enemyRect.y + enemyRect.h >= destRect.y)
    return true;
  else
    return false;
}

void Enemy::run() {
  state = EnemyStateRunning;
  runningTimer = ENEMY_RUNNING_TIMER_MAX;
}
