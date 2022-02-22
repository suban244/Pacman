#include "DialogueBox.h"
#include "Pacman.h"

Button::Button(std::string name, Callbacks onClick, SDL_Rect &baseRect)
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

void Button::render(bool isSelected) {
  if (isSelected)
    textureSelected.render(&textureSelectedRect);
  else
    texture.render(&textureRect);
}

DialogueBox::DialogueBox(std::string name) {
  int centerX = WINDOW_WIDTH / 2;
  int centerY = WINDOW_HEIGHT / 2;

  baseRect = {centerX - BUFFER, centerY - 2 * BUFFER, 2 * BUFFER, 2 * BUFFER};

  nameTexture.loadSentence(name, 32, Texture::White);
  nameTexture.queryTexture(nameRect.w, nameRect.h);
  baseRect.x -= nameRect.w / 2;
  baseRect.w += nameRect.w;
  baseRect.y -= nameRect.h / 2;
  baseRect.h += nameRect.h;
  nameRect.x = baseRect.x + BUFFER;
  nameRect.y = baseRect.y + BUFFER;

  buttonIndex = 0;
}

DialogueBox::~DialogueBox() {
  for (size_t i = 0; i < buttons.size(); i++) {
    delete buttons[i];
  }
}
void DialogueBox::addOption(std::string option, Callbacks onClick) {
  buttons.push_back(new Button(option, onClick, baseRect));
}
void DialogueBox::render() {
  SDL_SetRenderDrawColor(Game::renderer, 20, 20, 20, 255);
  SDL_RenderFillRect(Game::renderer, &baseRect);
  nameTexture.render(&nameRect);
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i]->render(i == buttonIndex);
  }
}

void DialogueBox::handleInput(SDL_Event &e, Pacman *pacmanObject) {
  switch (e.type) {
  case SDL_KEYDOWN:
    switch (e.key.keysym.sym) {
    case SDLK_RETURN:
      (pacmanObject->*buttons[buttonIndex]->onClick)();
      break;
    case SDLK_ESCAPE:
      (pacmanObject->*buttons.front()->onClick)();
      break;
    case SDLK_w:
      if (buttonIndex > 0)
        buttonIndex--;
      break;
    case SDLK_s:
      if (buttonIndex < buttons.size() - 1)
        buttonIndex++;
    default:
      break;
    }
  }
}
