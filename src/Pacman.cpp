#include "Pacman.h"
#include "Game.h"

Pacman::Pacman(StateMachine *s) : GameState(s), pacmanLocation(1, 1) {
  enemies.push_back(new Enemy(1, 1, ENEMY_RANDOM_STRAIGHT));
  enemies.push_back(new Enemy(1, 1, ENEMY_RANDOM_STRAIGHT));
  enemies.push_back(new Enemy(1, 1, ENEMY_DFS_LESS_BAD));
  enemies.push_back(new Enemy(23, 18, ENEMY_EUCLIDEAN));
  enemies.push_back(new Enemy(23, 1, ENEMY_BFS));

  for (int i = 0; i < 3; i++) {
    numTextures[i].loadSentence(std::to_string(i + 1), 128, Texture::White);
  }
  WonTexture.loadSentence("WON", 128, Texture::White);

  pacmanSprite.loadFromFile("assets/pacman.png");
}

Pacman::~Pacman() {
  for (Enemy *e : enemies)
    delete e;
}

int Pacman::BLOCK_SIZE = ENTITY_SIZE * 2;
int Pacman::gridStartPosX = WINDOW_WIDTH / 10 * 3;
int Pacman::gridStartPosY = 0;
int Pacman::pacmanSpriteSize = 80; // Becasue each sprite is 80 x 80

void Pacman::init() {
  pacmanSrcRect.h = pacmanSrcRect.w = pacmanSpriteSize;
  pacmanSrcRect.y = pacmanSrcRect.x = animationCount = 0;
  pacmanDestRect.x = 0;
  pacmanDestRect.y = gridStartPosY + BLOCK_SIZE + 5;
  pacmanDestRect.h = pacmanDestRect.w = ENTITY_SIZE;
  pacmanSrcRect.x = 160;
  pacmanNextDirection = pacmanDirection = DirectionRight;

  for (Enemy *e : enemies)
    e->init(gridStartPosX, gridStartPosY, BLOCK_SIZE, gameGrid);

  state = StateStarting;
  startingStateTime = STARTING_STATE_TIME;
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
  for (Enemy *e : enemies)
    e->render();

  pacmanLocation.calculateCoordinateToRender(pacmanDestRect, gridStartPosX,
                                             gridStartPosY, BLOCK_SIZE);
  pacmanSprite.renderEX(&pacmanDestRect, &pacmanSrcRect, pacmanDirection);

  if (state == StateStarting) {
    numTextures[startingStateTime / (STARTING_STATE_TIME / 3)].render(
        WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
  } else if (state == StateWon) {
    WonTexture.render(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
  }
}
void Pacman::update() {
  switch (state) {
  case StateStarting:
    startingStateTime--;
    if (startingStateTime < 0) {
      state = StatePlaying;
    }
    return;
  case StatePlaying: {
    for (Enemy *e : enemies)
      e->update(gameGrid, pacmanLocation);
    bool colision = false;
    for (Enemy *e : enemies)
      colision = e->detectColision(pacmanDestRect);
    if (colision)
      std::cout << "OH NO" << std::endl;

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
      state = StateWon;
    }

    animationCount++;
    pacmanSrcRect.x = 80 * ((animationCount / 10) % 3);
    break;
  }

  default:
    return;
  }
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
