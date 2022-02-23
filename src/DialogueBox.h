#pragma once
#include "Game.h"
#include <vector>
#define BUFFER 20

class Pacman;
typedef void (Pacman::*Callbacks)();

struct Button {
  std::string name;
  Callbacks onClick;
  Texture texture, textureSelected;
  SDL_Rect textureRect, textureSelectedRect;

  Button(std::string name, Callbacks onClick, SDL_Rect &baseRect);
  void render(bool isSelected = false);
  void updateName(std::string newName, SDL_Rect &baseRect);
};

class DialogueBox {
  SDL_Rect baseRect;

  Texture nameTexture;
  SDL_Rect nameRect;

  std::vector<Button *> buttons;
  size_t buttonIndex;

public:
  DialogueBox(std::string name);
  ~DialogueBox();
  void addOption(std::string option, Callbacks onClick);
  void updateOptionMessage(std::string oldMessage, std::string newMessage);

  void render();
  void update();
  void handleInput(SDL_Event &e, Pacman *pacmanObject);
};
