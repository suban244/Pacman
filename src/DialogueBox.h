#pragma once
#include "Game.h"
#include <vector>
#define BUFFER 20

class Pacman;
typedef void (Pacman::*Callbacks)();

struct Button {
  std::string name;
  Callbacks onClick;
  Texture texture, TextureSelected;
  SDL_Rect textureRect, textureSelectedRect;
  Button(std::string name, Callbacks onClick, SDL_Rect &baseRect);
  void render();
};

class DialogueBox {
  SDL_Rect baseRect;

  Texture nameTexture;
  SDL_Rect nameRect;

  std::vector<Button *> buttons;
  int buttonIndex;

public:
  DialogueBox(std::string name);
  ~DialogueBox();
  void addOption(std::string option, Callbacks onClick);

  void render();
  void update();
  void handleInput(SDL_Event &e, Pacman *pacmanObject);
};
