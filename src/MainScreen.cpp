#include "MainScreen.h"
#include "Game.h"
#include "GameState.h"
#include "Pacman.h"

bool buttonPress(SDL_Rect &rect, int x, int y)
{
  return x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h;
}

MainScreen::MainScreen(StateMachine *s) : GameState(s)
{
  pacmanIconRect.x = 400;
  pacmanIconRect.y = 30;
  pacmanIconRect.w = 500;
  pacmanIconRect.h = 100;
  pacmanIcon.loadFromFile("assets/pacmanIcon.png");

  textRect.x = 300;
  textRect.y = 200;
  textRect.w = 800;
  textRect.h = 300;
  text.loadFromFile("assets/new.jpg");

  soundTextRect.x = 370;
  soundTextRect.y = 470;
  soundTextRect.w = 500;
  soundTextRect.h = 200;
  soundText.loadFromFile("assets/sound.png");
}

void MainScreen::loadPacman()
{
  stateMachineRef->AddState(new Pacman(stateMachineRef));
}

void MainScreen::init()
{
  // startButtonRect.h = startButtonRect.w = startButtonRect.x =
  //   startButtonRect.y = 50;
}
void MainScreen::render()
{
  SDL_SetRenderDrawColor(Game::renderer, 100, 100, 100, 255);
  SDL_RenderFillRect(Game::renderer, &startButtonRect);
  pacmanIcon.render(&pacmanIconRect);
  text.render(&textRect);
  soundText.render(&soundTextRect);
}

void MainScreen::handleInput(SDL_Event &e)
{

  int x = e.button.x;
  int y = e.button.y;
  switch (e.type)
  {

  case SDL_KEYDOWN:
    switch (e.key.keysym.sym)
    {
    case SDLK_RETURN:
      loadPacman();
      break;
    case SDLK_m:
      /*
       * TODO:
       * Write code to switch on the sound
       */
      break;
    case SDLK_n:
      /*
       *TODO:
       * Write code to switch off the sound
       */
      break;
    }

    break;

  default:
    break;
  }
}
void MainScreen::update() {}
