#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

/*
 * Interface for the SDL_Texture
 */

class Texture {
  // static void DrawFillCircle(SDL_Renderer *renderer, int x, int y, int
  // radius);

public:
  static SDL_Color Black, Green, DarkGreen, Red, White;
  Texture();
  ~Texture();

  bool loadFromFile(std::string path);
  bool loadSentence(std::string sentence, int fontSize = 48,
                    SDL_Color sentenceColor = Black);

  void setColor(Uint8 red, Uint8 green, Uint8 blue);
  void setAlpha(Uint8 alpha);
  int getWidth();
  int getHeight();
  void queryTexture(int &width, int &height);

  void render(SDL_Rect *destRect, SDL_Rect *srcRect = NULL);
  void renderEX(SDL_Rect *destRect, SDL_Rect *srcRect = NULL, double angle = 0,
                SDL_Point *center = NULL,
                SDL_RendererFlip flip = SDL_FLIP_NONE);
  void render(int x, int y);

  static void DrawFillCircle(SDL_Renderer *renderer, int x, int y, int radius);
  void clean();

private:
  SDL_Texture *mTexture;
  int mWidth, mHeight;
};
