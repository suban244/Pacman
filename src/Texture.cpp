#include "Texture.h"
#include "Game.h"

SDL_Color Texture::Black = {0, 0, 0, 255};
SDL_Color Texture::Green = {0, 255, 0, 255};
SDL_Color Texture::Red = {255, 0, 0, 255};
SDL_Color Texture::DarkGreen = {118, 150, 86};

Texture::Texture() {
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

Texture::~Texture() { Texture::clean(); }

bool Texture::loadFromFile(std::string path) {
  /*
   * Loads a texture from the given path
   */

  Texture::clean();
  SDL_Surface *tempSurace = IMG_Load(path.c_str());
  mTexture = SDL_CreateTextureFromSurface(Game::renderer, tempSurace);
  SDL_FreeSurface(tempSurace);

  if (mTexture) {
    SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);
    return true;
  } else {
    return false;
  }
}

bool Texture::loadSentence(std::string sentence, int fontSize,
                           SDL_Color sentenceColor) {
  /*
   * Loads a texture from sentece
   */

  Texture::clean();

  TTF_Font *Roboto = TTF_OpenFont("assets/Roboto.ttf", fontSize);
  SDL_Surface *surfaceMessage =
      TTF_RenderText_Solid(Roboto, sentence.c_str(), sentenceColor);

  mTexture = SDL_CreateTextureFromSurface(Game::renderer, surfaceMessage);

  SDL_FreeSurface(surfaceMessage);
  TTF_CloseFont(Roboto);

  if (mTexture) {
    SDL_QueryTexture(mTexture, NULL, NULL, &mWidth, &mHeight);
    return true;
  } else {
    return false;
  }
}

// Sets the alpha for the texture
void Texture::setAlpha(Uint8 alpha) { SDL_SetTextureAlphaMod(mTexture, alpha); }

// Sets the colormod for the texture
void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

// Destroys the texture
void Texture::clean() {
  if (mTexture) {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = mHeight = 0;
  }
}

void Texture::render(SDL_Rect *destRect, SDL_Rect *srcRect) {
  /*
   * Renders a portion of the texture at a rect
   */
  SDL_RenderCopy(Game::renderer, mTexture, srcRect, destRect);
}
void Texture::renderEX(SDL_Rect *destRect, SDL_Rect *srcRect, double angle,
                       SDL_Point *center, SDL_RendererFlip flip) {
  /*
   * Renders a portion of the texture at a rect
   */
  SDL_RenderCopyEx(Game::renderer, mTexture, srcRect, destRect, angle, center,
                   flip);
}
void Texture::render(int x, int y) {
  /*
   * Renders the entire texture starting from a point
   * Size dpeneds on size of the texture
   */

  SDL_Rect tempRect = {x, y, mWidth, mHeight};
  SDL_RenderCopy(Game::renderer, mTexture, NULL, &tempRect);
}

int Texture::getWidth() { return mWidth; }
int Texture::getHeight() { return mHeight; }
void Texture::queryTexture(int &width, int &height) {
  width = getWidth();
  height = getHeight();
}
