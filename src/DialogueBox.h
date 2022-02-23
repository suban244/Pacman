#pragma once
#include "Game.h"
#include <vector>
#define BUFFER 20

class Pacman;
class MainScreen;

template <typename T> std::string type_name();

template <typename T> struct Button {
  std::string name;
  T onClick;
  Texture texture, textureSelected;
  SDL_Rect textureRect, textureSelectedRect;

  Button(std::string name, T onClick, SDL_Rect &baseRect)
      : name(name), onClick(onClick) {
    texture.loadSentence(name, 24, Texture::White);
    textureSelected.loadSentence(name, 28, Texture::Red);

    texture.queryTexture(textureRect.w, textureRect.h);
    textureSelected.queryTexture(textureSelectedRect.w, textureSelectedRect.h);

    textureRect.y = baseRect.y + baseRect.h - BUFFER / 2;
    textureSelectedRect.y = baseRect.y + baseRect.h - BUFFER / 2;

    baseRect.h += textureSelectedRect.h + BUFFER / 2;
    if (textureSelectedRect.w + 2 * BUFFER > baseRect.w) {
      baseRect.x = WINDOW_WIDTH / 2 - textureSelectedRect.w / 2 - BUFFER;
      baseRect.w = textureSelectedRect.w + 2 * BUFFER;
    }

    textureRect.x = WINDOW_WIDTH / 2 - textureRect.w / 2;
    textureSelectedRect.x = WINDOW_WIDTH / 2 - textureSelectedRect.w / 2;
  }
  void render(bool isSelected = false) {
    if (isSelected)
      textureSelected.render(&textureSelectedRect);
    else
      texture.render(&textureRect);
  }
  void updateName(std::string newName, SDL_Rect &baseRect) {
    name = newName;
    texture.loadSentence(name, 24, Texture::White);
    textureSelected.loadSentence(name, 28, Texture::Red);

    texture.queryTexture(textureRect.w, textureRect.h);
    textureSelected.queryTexture(textureSelectedRect.w, textureSelectedRect.h);

    textureRect.x = WINDOW_WIDTH / 2 - textureRect.w / 2;
    textureSelectedRect.x = WINDOW_WIDTH / 2 - textureSelectedRect.w / 2;

    if (textureSelectedRect.w + 2 * BUFFER > baseRect.w) {
      baseRect.x = WINDOW_WIDTH / 2 - textureSelectedRect.w / 2 - BUFFER;
      baseRect.w = textureSelectedRect.w + 2 * BUFFER;
    }
  }
};

template <typename baseClass> class DialogueBox {
  SDL_Rect baseRect;
  std::string name;

  Texture nameTexture;
  SDL_Rect nameRect;

  std::vector<Button<void (baseClass::*)()> *> buttons;
  size_t buttonIndex;

  SDL_Colour backgroundColor;

public:
  DialogueBox(std::string name = "",
              SDL_Color backgroundColor = {20, 20, 20, 255})
      : name(name), backgroundColor(backgroundColor) {
    int centerX = WINDOW_WIDTH / 2;
    int centerY = WINDOW_HEIGHT / 2;

    baseRect = {centerX - BUFFER, centerY - 2 * BUFFER, 2 * BUFFER, 2 * BUFFER};

    if (name != "") {
      nameTexture.loadSentence(name, 32, Texture::White);
      nameTexture.queryTexture(nameRect.w, nameRect.h);
      baseRect.x -= nameRect.w / 2;
      baseRect.w += nameRect.w;
      baseRect.y -= nameRect.h / 2;
      baseRect.h += nameRect.h;
      nameRect.x = baseRect.x + BUFFER;
      nameRect.y = baseRect.y + BUFFER;
    }

    buttonIndex = 0;
  }

  ~DialogueBox() {
    for (size_t i = 0; i < buttons.size(); i++) {
      delete buttons[i];
    }
  }
  void addOption(std::string option, void (baseClass::*onClick)()) {
    buttons.push_back(
        new Button<void (baseClass::*)()>(option, onClick, baseRect));
    // Move items up a little
    nameRect.y -= buttons.back()->textureSelectedRect.h / 2;
    baseRect.y -= buttons.back()->textureSelectedRect.h / 2;
    for (size_t i = 0; i < buttons.size(); i++) {
      buttons[i]->textureRect.y -= buttons.back()->textureSelectedRect.h / 2;
      buttons[i]->textureSelectedRect.y -=
          buttons.back()->textureSelectedRect.h / 2;
    }
  }
  void updateOptionMessage(std::string oldMessage, std::string newMessage) {
    for (size_t i = 0; i < buttons.size(); i++) {
      if (oldMessage == buttons[i]->name)
        buttons[i]->updateName(newMessage, baseRect);
    }
  }

  void render() {
    SDL_SetRenderDrawColor(Game::renderer, backgroundColor.r, backgroundColor.g,
                           backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(Game::renderer, &baseRect);

    if (name != "")
      nameTexture.render(&nameRect);

    for (size_t i = 0; i < buttons.size(); i++) {
      buttons[i]->render(i == buttonIndex);
    }
  }
  void update();
  void handleInput(SDL_Event &e, baseClass *ptr) {
    switch (e.type) {
    case SDL_KEYDOWN:
      switch (e.key.keysym.sym) {
      case SDLK_RETURN: {
        //(ptr->*buttons[buttonIndex]->onClick)();
        void (baseClass::*p)() = buttons[buttonIndex]->onClick;
        (ptr->*p)();
        break;
      }
      case SDLK_ESCAPE:
        (ptr->*buttons.front()->onClick)();
        break;
      case SDLK_w:
        if (buttonIndex > 0)
          buttonIndex--;
        else
          buttonIndex = buttons.size() - 1;
        break;
      case SDLK_s:
        if (buttonIndex < buttons.size() - 1)
          buttonIndex++;
        else
          buttonIndex = 0;
      default:
        break;
      }
    }
  }
};
