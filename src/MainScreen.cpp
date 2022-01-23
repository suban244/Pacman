#include "MainScreen.h"
#include "Game.h"
#include "GameState.h"
#include "Pacman.h"

bool buttonPress(SDL_Rect &rect, int x, int y) {
  return x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h;
}

MainScreen::MainScreen(StateMachine *s) : GameState(s) {}

void MainScreen::loadPacman() {
  stateMachineRef->AddState(new Pacman(stateMachineRef));
}

void MainScreen::init() {
  startButtonRect.h = startButtonRect.w = startButtonRect.x =
      startButtonRect.y = 50;
}
void MainScreen::render() {
  SDL_SetRenderDrawColor(Game::renderer, 100, 100, 100, 255);
  SDL_RenderFillRect(Game::renderer, &startButtonRect);
}

void MainScreen::handleInput(SDL_Event &e) {

  int x = e.button.x;
  int y = e.button.y;
  switch (e.type) {
  case SDL_MOUSEBUTTONDOWN:
    if (buttonPress(startButtonRect, x, y)) {
      std::cout << "click" << std::endl;
      loadPacman();
    }
    break;
  default:
    break;
  }
}
void MainScreen::update() {}
