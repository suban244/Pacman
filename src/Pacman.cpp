#include "Pacman.h"
#include "Game.h"

Pacman::Pacman(StateMachine *s)
    : GameState(s), pacmanLocation(1, 1), pause("Pause"), gameover("Game Over"),
      win("Win") {
  /*
  enemies.push_back(new Enemy(gameGrid, 1, 1, ENEMY_RANDOM_STRAIGHT));
  enemies.push_back(new Enemy(gameGrid, 1, 1, ENEMY_RANDOM));
  enemies.push_back(new Enemy(gameGrid, 10, 10, ENEMY_EUCLIDEAN));
  */

  enemies.push_back(new Enemy(gameGrid, 12, 10, ENEMY_BFS));
  enemies.push_back(new Enemy(gameGrid, 12, 10, ENEMY_ASTAR));
  enemies.push_back(new Enemy(gameGrid, 12, 10, ENEMY_DFS_LESS_BAD));

  for (int i = 0; i < 3; i++) {
    numTextures[i].loadSentence(std::to_string(i + 1), 128, Texture::White);
  }
  WonTexture.loadSentence("WON", 128, Texture::White);

  pacmanSprite.loadFromFile("assets/pacman.png");

  enemyRunningSprite.loadFromFile("assets/running.png");
  enemyResetingSprite.loadFromFile("assets/reset.png");

  chunkChomp = Mix_LoadWAV("assets/chomp.mp3");
  chunkPower = Mix_LoadWAV("assets/powerup.mp3");
  chunkEatEnemy = Mix_LoadWAV("assets/eat_enemy.mp3");
  chunkGameover = Mix_LoadWAV("assets/gameover.mp3");
  chunkDeath = Mix_LoadWAV("assets/death.mp3");
  music = Mix_LoadMUS("assets/donkeyKongMusic.mp3");
  Mix_VolumeMusic(VOLUME_PLAYING);

  scoreTexture.loadSentence("Score", 32, Texture::White);
  scoreTexture.queryTexture(scoreRect.w, scoreRect.h);
  scoreAmountTexture.loadSentence(std::to_string(0), 32, Texture::White);

  pause.addOption("Continue", &Pacman::togglePause);
  pause.addOption("Main Menu", &Pacman::returnToMainScreen);
  pause.addOption("Quit", &Pacman::quit);

  gameover.addOption("Retry", &Pacman::init);
  gameover.addOption("Quit", &Pacman::quit);
  gameover.addOption("Main Menu", &Pacman::returnToMainScreen);

  win.addOption("Play Again", &Pacman::init);
  win.addOption("Main Menu", &Pacman::returnToMainScreen);
  win.addOption("Quit", &Pacman::quit);
}

Pacman::~Pacman() {
  for (Enemy *e : enemies)
    delete e;
  Mix_FreeChunk(chunkChomp);
  Mix_FreeChunk(chunkDeath);
  Mix_FreeChunk(chunkEatEnemy);
  Mix_FreeChunk(chunkPower);
}

int Pacman::pacmanSpriteSize = 80; // Becasue each sprite is 80 x 80

void Pacman::init() {
  lives = 3;

  score = 0;
  oldScore = score;
  scoringWindow = -1;
  scoreFactor = 1;

  for (Enemy *e : enemies) {
    e->init(gameGrid);
  }

  pacmanSrcRect.h = pacmanSrcRect.w = pacmanSpriteSize;
  pacmanSrcRect.y = pacmanSrcRect.x = animationCount = 0;
  pacmanDestRect.x = 0;
  pacmanDestRect.y = gameGrid.startPosY + gameGrid.BLOCK_SIZE + 5;
  pacmanDestRect.h = pacmanDestRect.w = ENTITY_SIZE;
  pacmanSrcRect.x = 160;
  pacmanNextDirection = pacmanDirection = DirectionRight;

  state = StateStarting;
  startingStateTime = STARTING_STATE_TIME;

  Mix_PlayMusic(music, -1);
  scoreAmountTexture.loadSentence(std::to_string(score), 32, Texture::White);
  scoreAmountTexture.queryTexture(scoreAmountRect.w, scoreAmountRect.h);
  gameGrid.reset();
}
void Pacman::render() {
  Uint8 alpha = state == StatePause ? 100 : 255;

  SDL_Rect boxRect;
  boxRect.x = gameGrid.startPosX;
  boxRect.y = gameGrid.startPosY;
  boxRect.w = boxRect.h = gameGrid.BLOCK_SIZE;
  for (int i = 0; i < GRID_HEIGHT; i++) {
    for (int j = 0; j < GRID_WIDTH; j++) {

      switch (gameGrid.nodes[i][j].state) {
      case NodeStateWall:
        SDL_SetRenderDrawColor(Game::renderer, 0, 200, 50, alpha);
        break;
      default:
        SDL_SetRenderDrawColor(Game::renderer, 0, 100, 0, alpha);
        break;
      }
      SDL_RenderFillRect(Game::renderer, &boxRect);

      SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, alpha);

      switch (gameGrid.nodes[i][j].state) {
      case NodeStatePoint:
        Texture::DrawFillCircle(Game::renderer, boxRect.x + boxRect.w / 2,
                                boxRect.y + boxRect.w / 2, 2);
        break;
      case NodeStatePower:
        Texture::DrawFillCircle(Game::renderer, boxRect.x + boxRect.w / 2,
                                boxRect.y + boxRect.w / 2, 5);
        break;
      default:
        break;
      }

      boxRect.x += gameGrid.BLOCK_SIZE;
    }
    boxRect.x = gameGrid.startPosX;
    boxRect.y += gameGrid.BLOCK_SIZE;
  }

  if (state != StateJustDied) {
    for (Enemy *e : enemies)
      e->render(gameGrid, enemyRunningSprite, enemyResetingSprite, alpha);

    pacmanLocation.calculateCoordinateToRender(
        pacmanDestRect, gameGrid.startPosX, gameGrid.startPosY,
        gameGrid.BLOCK_SIZE);

    pacmanSprite.setAlpha(alpha);
    pacmanSprite.renderEX(&pacmanDestRect, &pacmanSrcRect, pacmanDirection);
  }

  if (state == StateStarting) {
    numTextures[startingStateTime / (STARTING_STATE_TIME / 3)].render(
        WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
  } else if (state == StateWon) {
    WonTexture.render(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
  }

  // Rendering Lives
  boxRect.h = boxRect.w = pacmanDestRect.h;
  boxRect.x = gameGrid.startPosX + gameGrid.BLOCK_SIZE * GRID_WIDTH + boxRect.h;
  boxRect.y = gameGrid.startPosY;
  SDL_Rect spriteRect = {80, 0, 80, 80};
  for (int i = 0; i < lives; i++) {
    pacmanSprite.render(&boxRect, &spriteRect);
    boxRect.x += boxRect.h;
  }

  // Rendering Score
  scoreRect.y = boxRect.y + 2 * boxRect.h;
  scoreRect.x =
      gameGrid.startPosX + gameGrid.BLOCK_SIZE * GRID_WIDTH + boxRect.h;
  scoreTexture.render(&scoreRect);
  scoreAmountRect.y = scoreRect.y + 20;
  scoreAmountRect.x = scoreRect.x;
  scoreAmountTexture.render(&scoreAmountRect);

  if (state == StatePause) {
    pause.render();
  } else if (state == StateLost) {
    gameover.render();
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
    for (Enemy *e : enemies) {
      colision = e->detectColision(pacmanDestRect);
      if (colision) {
        if (e->state == EnemyStateChasing) {
          resetBoard();
          break;
        } else {
          Mix_PlayChannel(-1, chunkEatEnemy, 0);

          if (scoringWindow >= 0 && e->state != EnemyStateReseting) {
            score += scoreFactor;
            scoreFactor *= 2;
          }
          e->state = EnemyStateReseting;
        }
      }
    }

    if (scoringWindow >= 0) {
      scoringWindow--;
    } else {
      scoreFactor = 1;
    }

    if (pacmanLocation.atCenter()) {

      NodeState nstate =
          gameGrid.consume(pacmanLocation.blockY, pacmanLocation.blockX);
      if (nstate == NodeStatePoint)
        Mix_PlayChannelTimed(-1, chunkChomp, 0, 300);
      else if (nstate == NodeStatePower)
        Mix_PlayChannel(-1, chunkPower, 0);

      if (nstate == NodeStatePower) {
        for (Enemy *e : enemies) {
          e->run();
        }
        scoringWindow = ENEMY_RUNNING_TIMER_MAX;
      }

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
    pacmanSrcRect.x = 80 * ((animationCount / 5) % 5);
    break;
  }

  case StateJustDied:
    pauseTime--;
    if (pauseTime <= 0) {
      state = StateStarting;
      startingStateTime = STARTING_STATE_TIME;
    }
    break;

  case StatePause:
    break;

  default:
    return;
  }
  if (oldScore != score) {
    oldScore = score;
    scoreAmountTexture.loadSentence(std::to_string(score), 32, Texture::White);
    scoreAmountTexture.queryTexture(scoreAmountRect.w, scoreAmountRect.h);
  }
}

void Pacman::resetBoard() {
  lives--;

  pacmanGoToDefaultLocation();
  for (Enemy *e : enemies)
    e->init(gameGrid);

  state = StateJustDied;
  pauseTime = JUST_DIED_PAUSE_TIME;

  if (lives == 0) {
    state = StateLost;
    Mix_PlayChannel(-1, chunkGameover, 0);
  } else {
    Mix_PlayChannel(-1, chunkDeath, 0);
  }
}

void Pacman::pacmanGoToDefaultLocation() {
  pacmanLocation.blockX = pacmanLocation.blockY = 1;
  pacmanLocation.offsetX = pacmanLocation.offsetY = 0;
}
void Pacman::handleInput(SDL_Event &e) {
  if (state == StatePlaying) {
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
      case SDLK_ESCAPE:
        togglePause();
        break;
      default:
        break;
      }

      break;
    default:
      break;
    }
  } else if (state == StateWon) {
    if (e.type == SDL_KEYDOWN) {
      gameGrid.reset();
      init();
      pacmanGoToDefaultLocation();
    }
  } else if (state == StatePause) {
    pause.handleInput(e, this);
  } else if (state == StateLost) {
    gameover.handleInput(e, this);
  }
}
// Maybe use a tempr to track the direction for some time
/*
 * So we have a variable that will denote where to chagne the direction
 * At center we update the direction and do stuff
 * Upon block change we reest the direction
 */

void Pacman::togglePause() {
  if (state == StatePause) {
    state = stateBeforePause;
    Mix_VolumeMusic(VOLUME_PLAYING);
  } else {
    stateBeforePause = state;
    state = StatePause;
    Mix_VolumeMusic(VOLUME_PAUSED);
  }
}

void Pacman::returnToMainScreen() { stateMachineRef->popTopState(); }
void Pacman::quit() { stateMachineRef->quit(); }
