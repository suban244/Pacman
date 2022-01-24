#pragma once
#include "GameState.h"
#include "Structures.h"

class Pacman : public GameState {
private:
  enum Direction {
    DirectionRight = 0,
    DirectionDown = 90,
    DirectionLeft = 180,
    DirectionUp = 270

  };
  Texture pacmanSprite;
  EntityLocation pacmanLocation;
  SDL_Rect pacmanSrcRect, pacmanDestRect;
  Direction pacmanDirection, pacmanNextDirection;
  int animationCount;

  Grid gameGrid;
  static int BLOCK_SIZE, gridStartPosX, gridStartPosY, pacmanSpriteSize;

  bool canMove(Direction direciton, bool checkOffSet = true);

public:
  Pacman(StateMachine *s);
  void init() override;
  void handleInput(SDL_Event &event) override;
  void update() override;
  void render() override;
};
