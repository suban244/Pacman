#include "Pacman.h"
#include "Game.h"

/*
 * Map of 19 x 19
 * So we need a 19 x 19 ko array of nodes
 *
 */

Pacman::Pacman(StateMachine *s)
    : GameState(s), pacmanLocation(1, 1), e1(0, 10, ENEMY_RANDOM_STRAIGHT),
      e2(0, 5, ENEMY_RANDOM), e3(0, 18, ENEMY_DFS_LESS_BAD),
      e4(14, 11, ENEMY_EUCLIDEAN) {}
int Pacman::BLOCK_SIZE = 40;
int Pacman::gridStartPosX = WINDOW_WIDTH / 10 * 3;
int Pacman::gridStartPosY = WINDOW_HEIGHT / 10;
int Pacman::pacmanSpriteSize = 80; // Becasue each sprite is 80 x 80

void Pacman::init() {
  pacmanSprite.loadFromFile("assets/pacman.png");
  pacmanSrcRect.h = pacmanSrcRect.w = pacmanSpriteSize;
  pacmanSrcRect.y = pacmanSrcRect.x = animationCount = 0;
  pacmanDestRect.x = 0;
  pacmanDestRect.y = gridStartPosY + BLOCK_SIZE + 5;
  pacmanDestRect.h = pacmanDestRect.w = ENTITY_SIZE;
  pacmanSrcRect.x = 160;
  pacmanNextDirection = pacmanDirection = DirectionRight;
  e1.init(gridStartPosX, gridStartPosY, BLOCK_SIZE);
  e2.init(gridStartPosX, gridStartPosY, BLOCK_SIZE);
  e3.init(gridStartPosX, gridStartPosY, BLOCK_SIZE);
  e4.init(gridStartPosX, gridStartPosY, BLOCK_SIZE);
}
void Pacman::render() {
  SDL_Rect boxRect;
  boxRect.x = gridStartPosX;
  boxRect.y = gridStartPosY;
  boxRect.w = boxRect.h = BLOCK_SIZE;
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {

      switch (gameGrid.nodes[i][j].state) {
      case NodeStateWall:
        SDL_SetRenderDrawColor(Game::renderer, 0, 200, 50, 255);
        break;
      default:
        SDL_SetRenderDrawColor(Game::renderer, 0, 100, 0, 255);
        break;
      }
      SDL_RenderFillRect(Game::renderer, &boxRect);

      SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
      switch (gameGrid.nodes[i][j].state) {
      case NodeStatePoint:
        Texture::DrawFillCircle(Game::renderer, boxRect.x + boxRect.w / 2,
                                boxRect.y + boxRect.w / 2, 2);
        break;
      case NodeStatePower:
        Texture::DrawFillCircle(Game::renderer, boxRect.x + boxRect.w / 2,
                                boxRect.y + boxRect.w / 2, 5);
        break;
      }

      boxRect.x += BLOCK_SIZE;
    }
    boxRect.x = gridStartPosX;
    boxRect.y += BLOCK_SIZE;
  }
  e1.render();
  e2.render();
  e3.render();
  e4.render();
  pacmanLocation.calculateCoordinateToRender(pacmanDestRect, gridStartPosX,
                                             gridStartPosY, BLOCK_SIZE);
  pacmanSprite.renderEX(&pacmanDestRect, &pacmanSrcRect, pacmanDirection);
}
void Pacman::update() {

  e1.update(gameGrid, pacmanLocation);
  e2.update(gameGrid, pacmanLocation);
  e3.update(gameGrid, pacmanLocation);
  e4.update(gameGrid, pacmanLocation);

  if (pacmanLocation.atCenter()) {
    gameGrid.consume(pacmanLocation.blockY, pacmanLocation.blockX);
    if (Grid::canMove(pacmanLocation, gameGrid, pacmanNextDirection)) {
      pacmanDirection = pacmanNextDirection;
      pacmanLocation.move(pacmanDirection);
    } else
      pacmanNextDirection = pacmanDirection;
  } else {
    if (Grid::canMove(pacmanLocation, gameGrid, pacmanDirection)) {
      pacmanLocation.move(pacmanDirection);
    }
  }
  if (gameGrid.complete()) {
  }

  animationCount++;
  pacmanSrcRect.x = 80 * ((animationCount / 10) % 3);
}
void Pacman::handleInput(SDL_Event &e) {
  switch (e.type) {
  case SDL_KEYDOWN:
    switch (e.key.keysym.sym) {
    case SDLK_w:
      pacmanNextDirection = DirectionUp;
      break;
    case SDLK_a:
      pacmanNextDirection = DirectionLeft;
      break;
    case SDLK_s:
      pacmanNextDirection = DirectionDown;
      break;
    case SDLK_d:
      pacmanNextDirection = DirectionRight;
      break;
    default:
      break;
    }

    break;
  default:
    break;
  }
}
// Maybe use a tempr to track the direction for some time
/*
 * So we have a variable that will denote where to chagne the direction
 * At center we update the direction and do stuff
 * Upon block change we reest the direction
 */
