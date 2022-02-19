#pragma once
#define SDL_MAIN_HANDLED
#include "MainScreen.h"
#include "StateMachine.h"
#include <iostream>

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280
#define FPS 60

class Game {

public:
  Game();
  ~Game();

  void init(const char *title, int xpos, int ypos, int width, int height,
            bool fullscreen);

  void handleEvents();
  void update();
  void render();
  void clean();
  void exitGame();

  bool running();

  static SDL_Renderer *renderer;
  static bool playMusic, playSoundEffect;

  void createGameBoard(std::string name0, std::string name1,
                       int _startTimeInMinutes = 10, bool _useEngine = false);
  void goBackToGameMenu();

private:
  bool isRunning;
  SDL_Window *window;

  StateMachine stateMachine;
  int count = 0;
};
