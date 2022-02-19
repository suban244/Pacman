#pragma once
#include "GameState.h"

/*
 * TODO: Time consuming
 * Make the main menu pretty
 */
class MainScreen : public GameState
{
private:
  SDL_Rect startButtonRect;
  void loadPacman();
  Texture pacmanIcon;
  SDL_Rect pacmanIconRect;
  Texture text;
  SDL_Rect textRect;
  Texture soundText;
  SDL_Rect soundTextRect;

public:
  MainScreen(StateMachine *s);
  void init() override;
  void handleInput(SDL_Event &event) override;
  void update() override;
  void render() override;
};
