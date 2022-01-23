#include "Game.h"
#include "Texture.h"
#include <SDL2/SDL_mixer.h>

SDL_Renderer *Game::renderer = nullptr;

Game::Game() {}

Game::~Game() { clean(); }

void Game::init(const char *title, int xpos, int ypos, int width, int height,
                bool fullscreen) {

  // By default we have no flags
  int flags = 0;
  if (fullscreen) {
    // If fullscreen, flag -> make window fullscreen
    flags = SDL_WINDOW_FULLSCREEN;
  }

  // We Start SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    // Window Creation
    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

    // Setting window color
    if (window) {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    // Creating renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer) {
      std::cout << "renderer created" << std::endl;
    }
    if (((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)))
      printf("Success\n");
    // Opacity power
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
      printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
             Mix_GetError());
    }

    if (TTF_Init() == -1) {
      printf("TTF_Init: %s\n", TTF_GetError());
      exit(2);
    }

    isRunning = true;
    // SDL_StartTextInput();
    stateMachine.AddState(new MainScreen(&stateMachine));
    stateMachine.processStateChanges();
  } else {
    isRunning = false;
  }
}

void Game::handleEvents() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT: {
    isRunning = false;
    break;
  }

  default:
    stateMachine.getActiveState()->handleInput(event);
    break;
  };
}

void Game::update() {
  count++;
  stateMachine.processStateChanges();
  stateMachine.getActiveState()->update();
}
void Game::render() {

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Do Rendering
  stateMachine.getActiveState()->render();

  SDL_RenderPresent(renderer);
}
void Game::clean() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();

  std::cout << "Game cleared" << std::endl;
}

void Game::exitGame() { this->isRunning = false; }

bool Game::running() { return isRunning; }
