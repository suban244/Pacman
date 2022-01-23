#pragma once
#include "Texture.h"
#include <SDL2/SDL.h>
class StateMachine;

class GameState {
protected:
  StateMachine *stateMachineRef;

public:
  GameState(StateMachine *s) : stateMachineRef(s) {}
  virtual ~GameState() {}

  virtual void init() = 0;
  virtual void handleInput(SDL_Event &event) = 0;
  virtual void update() = 0;
  virtual void render() = 0;
};
