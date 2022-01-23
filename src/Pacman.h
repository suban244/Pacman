#pragma once
#include "GameState.h"
#include "Structures.h"

class Pacman : public GameState {
private:
  Texture pacmanSprite;
  EntityLocation pacmanLocation;
  SDL_Rect pacmanSrcRect, pacmanDestRect;
  int animationCount;

  Grid gameGrid;
  static int BLOCK_SIZE, gridStartPosX, gridStartPosY, pacmanSpriteSize;

public:
  Pacman(StateMachine *s);
  void init() override;
  void handleInput(SDL_Event &event) override;
  void update() override;
  void render() override;
};
