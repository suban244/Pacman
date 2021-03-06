#pragma once
#include "Structures.h"
#include "Texture.h"
#include <algorithm>
#include <ctime>
#include <math.h>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#define ENTITY_SIZE 14
#define ENEMY_RUNNING_TIMER_MAX 600
#define ENEMY_FAR_SIGHTED_THRESHOLD 8

enum EnemyType {
  ENEMY_MEEP,        // He cute, likes pacman, very fast use DFS_LESS_BAD
  ENEMY_FAR_SIGHTED, // NORMAL speed, when pacman to enemy distance less than
                     // thresshold, random movement
  ENEMY_ELEPHANT,    // SLOW BUT SUPER ACCURATW
  ENEMY_HELPER,      // Path finds to a square near pacman
};

enum EnemyState { EnemyStateRunning, EnemyStateChasing, EnemyStateReseting };

struct Enemy {
  Texture texture;
  EntityLocation baseLocation, location;
  SDL_Rect destRect;
  Direction direction;
  EnemyState state;

  SDL_Rect srcRect;
  int animationCount;
  int runningTimer;

  std::vector<Node *> pathToBeFollowed;
  EnemyType type;

  Enemy(Grid &gameGrid, int i, int j, EnemyType e);
  /*
   * sets some variables for rendering purposesses
   */
  void init(Grid &gameGrid);

  /*
   * @param enemyRect SDL_Rect of the adversary (pacman)
   * Returns true if there is collision between destRect and enemyRect
   * (ie overlap)
   */
  bool detectColision(SDL_Rect &enemyRect);

  /*
   * Moves the Enemy object in a certain direction
   * Then updates where we shound render it
   */
  void update(Grid &gameGrid, EntityLocation &pacmanLocation);

  void render(const Grid &gameGrid, Texture &enemyRunningSprite,
              Texture &enemyResetingSprite, Uint8 alpha = 255);

  void run();

private:
  /*
   * Generates a random movable direction
   * @param gameGrid The gird to use as reference
   * Returns a movable direction
   */
  Direction getRandomDirection(Grid &gameGrid);

  /*
   * Performs a depth first search. The start is assumed to be the object's
   * location property
   * @param gameGrid The grid on which DFS is performed
   * @param pacmanLocation The destination of the DFS.
   * @param pathFollowed a vector where the path generated by dfs is to be
   * stored. Here the first element is the starting node and the last element is
   * the destination node
   * Returns the direction of from the first node to 2nd node. If the path is of
   * length 2 or greater. Else returns a direction using getRandomDirection().
   * Stores the entire path generated in @params: pathFollowed
   */
  Direction DFS_search(Grid &gameGrid, EntityLocation &pacmanLocation,
                       std::vector<Node *> &pathFollowed);

  /*
   * Performs a bredth first search. The start is assumed to be the object's
   * location property
   * @param gameGrid The grid on which DFS is performed
   * @param pacmanLocation The destination of the DFS.
   * @param pathFollowed a vector where the path generated by dfs is to be
   * stored. Here the first element is the starting node and the last element is
   * the destination node
   * Returns the direction of from the first node to 2nd node. If the path is of
   * length 2 or greater. Else returns a direction using getRandomDirection().
   * Stores the entire path generated in @params: pathFollowed
   */
  Direction BFS_sarech(Grid &gameGrid, EntityLocation &pacmanLocation,
                       std::vector<Node *> &pathFollowed);

  Direction AStarSearch(Grid &gameGrid, EntityLocation &pacmanLocation,
                        std::vector<Node *> &pathFollowed);

  Direction Dijkstra(Grid &gameGrid, EntityLocation &pacmanLocation,
                     std::vector<Node *> &pathFollowed);

  /*
   * Completely handles movement
   * @param gameGrid The grid of the game
   * If entity is at the center of a block, randomly changes entity's direction
   * by calling getRandomDirection()
   * Else continues entity in it's current direction
   */
  void moveFullyRandom(Grid &gameGrid);
  /*
   * Completely handles movement
   * @param gameGrid The grid of the game
   * If entity is at the center of a block and cannot move forward, changes the
   * entity's direction to a valid direciotn else continues.
   * Entity in it's current direction
   */
  void moveStrainghtRandom(Grid &gameGrid);

  /*
   * Moves the entity with the help of DFS_search()
   * @param gameGrid The grid of the game
   * @param pacmanLocation The destination of the DFS
   * Performs a DFS search whenever the entity is in the center of the block.
   * And then moves pacman in the direction suggested by DFS
   */
  void moveWithDFS(Grid &gameGrid, EntityLocation &pacmanLocation);

  /*
   * Moves the entity with the help of DFS_search()
   * @param gameGrid The grid of the game
   * @param pacmanLocation The destination of the DFS
   * Uses pathToBeFollowed as the reference for movement
   * If pathToBeFollowed is empty calls DFS_search() to generate a path
   * else using pathToBeFollowed to figure out which direction to go.
   * Does goes through the entire path suggested by an old DFS_search even when
   * destination my have changed
   */
  void moveWithDFS2(Grid &gameGrid, EntityLocation &pacmanLocation);

  /*
   * Moves the entity with the help of BFS_sarech()
   * @param gameGrid The grid of the game
   * @param pacmanLocation The destination of the BFS
   * Performs a DFS search whenever the entity is in the center of the block.
   * And then moves pacman in the direction suggested by DFS
   */
  void moveWithBFS(Grid &gameGrid, EntityLocation &pacmanLocation);
  void moveWithAStar(Grid &gameGrid, EntityLocation &pacmanLocation);

  void moveWithEuclideanDistance(Grid &gameGrid,
                                 EntityLocation &pacmanLocation);

  void runWithEculideanDistance(Grid &gameGrid, EntityLocation &pacmanLocation);

  /*
   * Enemy runs to its home square
   */
  void reset();
  bool inBaseLocation();

  void moveEnemyMeep(Grid &gameGrid, EntityLocation &pacmanLocation);
  void moveEnemyFarSighted(Grid &gameGrid, EntityLocation &pacmanLocation);
  void moveEnemyElephant(Grid &gameGrid, EntityLocation &pacmanLocation);
  void moveEnemyHelper(Grid &gameGrid, EntityLocation &pacmanLocation);
};
