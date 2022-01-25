#pragma once
#include "GameState.h"

/*
 * TODO: Time consuming
 * Make the main menu pretty
 */
class MainScreen : public GameState {
private:
  SDL_Rect startButtonRect;
  void loadPacman();

public:
  MainScreen(StateMachine *s);
  void init() override;
  void handleInput(SDL_Event &event) override;
  void update() override;
  void render() override;
};
