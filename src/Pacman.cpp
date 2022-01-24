#include "Pacman.h"
#include "Game.h"

/*
 * Map of 19 x 19
 * So we need a 19 x 19 ko array of nodes
 *
 */

Pacman::Pacman(StateMachine *s) : GameState(s), pacmanLocation(1, 1) {}
int Pacman::BLOCK_SIZE = 30;
int Pacman::gridStartPosX = WINDOW_WIDTH / 10 * 3;
int Pacman::gridStartPosY = WINDOW_HEIGHT / 10;
int Pacman::pacmanSpriteSize = 80; // Becasue each sprite is 80 x 80

void Pacman::init() {
  pacmanSprite.loadFromFile("assets/pacman.png");
  pacmanSrcRect.h = pacmanSrcRect.w = pacmanSpriteSize;
  pacmanSrcRect.y = pacmanSrcRect.x = animationCount = 0;
  pacmanDestRect.x = 0;
  pacmanDestRect.y = gridStartPosY + BLOCK_SIZE + 5;
  pacmanDestRect.h = pacmanDestRect.w = 20;
  pacmanSrcRect.x = 160;
  pacmanNextDirection = pacmanDirection = DirectionRight;
}
void Pacman::render() {
  SDL_Rect boxRect;
  boxRect.x = gridStartPosX;
  boxRect.y = gridStartPosY;
  boxRect.w = boxRect.h = BLOCK_SIZE;
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {

      switch (gameGrid.nodes[i][j].state) {
      case 0:
        SDL_SetRenderDrawColor(Game::renderer, 0, 200, 50, 255);
        break;
      case 1:
        SDL_SetRenderDrawColor(Game::renderer, 0, 100, 0, 255);
        break;
      default:
        break;
      }
      SDL_RenderFillRect(Game::renderer, &boxRect);
      boxRect.x += BLOCK_SIZE;
    }
    boxRect.x = gridStartPosX;
    boxRect.y += BLOCK_SIZE;
  }
  pacmanLocation.calculateCoordinateToRender(pacmanDestRect, gridStartPosX,
                                             gridStartPosY, BLOCK_SIZE);
  pacmanSprite.renderEX(&pacmanDestRect, &pacmanSrcRect, pacmanDirection);
}
void Pacman::update() {
  if (pacmanLocation.atCenter()) {
    pacmanDirection = pacmanNextDirection;
  }
  // bool blockChange = false;
  switch (pacmanDirection) {
  case DirectionUp:
    if (canMove(DirectionUp))
      pacmanLocation.move(false, false);
    break;
  case DirectionLeft:
    if (canMove(DirectionLeft))
      pacmanLocation.move(false, true);
    break;
  case DirectionDown:
    if (canMove(DirectionDown))
      pacmanLocation.move(true, false);
    break;
  case DirectionRight:
    if (canMove(DirectionRight))
      pacmanLocation.move(true, true);
    break;
  }
  animationCount++;
  pacmanSrcRect.x = 80 * ((animationCount / 10) % 3);
}
void Pacman::handleInput(SDL_Event &e) {
  switch (e.type) {
  case SDL_KEYDOWN:
    switch (e.key.keysym.sym) {
    case SDLK_w:
      if (canMove(DirectionUp)) {
        pacmanDirection = DirectionUp;
        pacmanNextDirection = DirectionUp;

      } else if (canMove(DirectionUp, false))
        pacmanNextDirection = DirectionUp;
      break;
    case SDLK_a:
      if (canMove(DirectionLeft)) {
        pacmanDirection = DirectionLeft;
        pacmanNextDirection = DirectionLeft;
      } else if (canMove(DirectionLeft, false))
        pacmanNextDirection = DirectionLeft;
      break;
    case SDLK_s:
      if (canMove(DirectionDown)) {
        pacmanDirection = DirectionDown;
        pacmanNextDirection = DirectionDown;
      } else if (canMove(DirectionDown, false))
        pacmanNextDirection = DirectionDown;
      break;
    case SDLK_d:
      if (canMove(DirectionRight)) {
        pacmanDirection = DirectionRight;
        pacmanNextDirection = DirectionRight;
      } else if (canMove(DirectionRight, false))
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
bool Pacman::canMove(Direction direction, bool checkOffSet) {
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
