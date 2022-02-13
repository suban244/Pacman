#pragma once
#include "Enemy.h"
#include "GameState.h"
#include "Structures.h"
#define STARTING_STATE_TIME 120

class Pacman : public GameState {
private:
  State state;

  Texture pacmanSprite;
  EntityLocation pacmanLocation;
  SDL_Rect pacmanSrcRect, pacmanDestRect;
  Direction pacmanDirection, pacmanNextDirection;
  int animationCount;

  std::vector<Enemy *> enemies;
  Grid gameGrid;
  static int pacmanSpriteSize;

  int startingStateTime;
  Texture numTextures[3], WonTexture;

public:
  Pacman(StateMachine *s);
  ~Pacman();
  void init() override;
  void handleInput(SDL_Event &event) override;
  void update() override;
  void render() override;
};
