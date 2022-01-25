#include "Pacman.h"
#include "Game.h"

/*
 * Map of 19 x 19
 * So we need a 19 x 19 ko array of nodes
 *
 */

Pacman::Pacman(StateMachine *s)
    : GameState(s), pacmanLocation(1, 1), e1(1, 10) {}
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
      default:
        SDL_SetRenderDrawColor(Game::renderer, 0, 100, 0, 255);
        break;
      }
      SDL_RenderFillRect(Game::renderer, &boxRect);

      SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
      switch (gameGrid.nodes[i][j].state) {
      case 2:
        Texture::DrawFillCircle(Game::renderer, boxRect.x + boxRect.w / 2,
                                boxRect.y + boxRect.w / 2, 2);
        break;
      case 3:
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
  pacmanLocation.calculateCoordinateToRender(pacmanDestRect, gridStartPosX,
                                             gridStartPosY, BLOCK_SIZE);
  pacmanSprite.renderEX(&pacmanDestRect, &pacmanSrcRect, pacmanDirection);
}
void Pacman::update() {
  if (e1.location.atCenter()) {
    e1.DFS_search(gameGrid);
  }
  e1.update();

  if (pacmanLocation.atCenter()) {
    gameGrid.consume(pacmanLocation.blockY, pacmanLocation.blockX);
    if (canMove(pacmanNextDirection)) {
      pacmanDirection = pacmanNextDirection;
      pacmanLocation.move(pacmanDirection);
    } else
      pacmanNextDirection = pacmanDirection;
  } else {
    if (canMove(pacmanDirection)) {
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
